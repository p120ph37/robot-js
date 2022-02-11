#include "ImageAdapter.h"
#include "ColorAdapter.h"
#include "PointAdapter.h"
#include "SizeAdapter.h"
#include <sstream>

Napi::Function ImageAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Image", {
    InstanceMethod("isValid", &ImageAdapter::isValid),
    InstanceMethod("create", &ImageAdapter::create),
    InstanceMethod("destroy", &ImageAdapter::destroy),
    InstanceMethod("getWidth", &ImageAdapter::getWidth),
    InstanceMethod("getHeight", &ImageAdapter::getHeight),
    InstanceMethod("getLength", &ImageAdapter::getLength),
    InstanceMethod("getData", &ImageAdapter::getData),
    InstanceMethod("getLimit", &ImageAdapter::getLimit),
    InstanceMethod("getPixel", &ImageAdapter::getPixel),
    InstanceMethod("setPixel", &ImageAdapter::setPixel),
    InstanceMethod("fill", &ImageAdapter::fill),
    InstanceMethod("swap", &ImageAdapter::swap),
    InstanceMethod("flip", &ImageAdapter::flip),
    InstanceMethod("eq", &ImageAdapter::eq),
    InstanceMethod("ne", &ImageAdapter::ne),
    InstanceMethod("toString", &ImageAdapter::toString),
  });
}

ImageAdapter::ImageAdapter(const Napi::CallbackInfo& info) :
  ClassAdapter(info),
  bufferRef(Napi::Reference<Napi::Value>::New(env.Global())) {

  if(WrapAdaptee(info) /*|| ConstructDefault(info) || CopyThat(info)*/) return;
  if(IsInstance(info[0])) { // make a copy, not a move, since we will track buffers separately.
    adaptee = std::make_shared<Robot::Image>(*Unwrap(info[0])->adaptee);
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = std::make_shared<Robot::Image>();
    return;
  }
  adaptee = std::make_shared<Robot::Image>(SizeAdapter::NewAdaptee(info));
}

Napi::Value ImageAdapter::isValid(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee->IsValid());
}

Napi::Value ImageAdapter::create(const Napi::CallbackInfo& info) {
  auto newSize = SizeAdapter::NewAdaptee(info);
  if(newSize.IsEmpty()) {
    return Napi::Boolean::New(env, false);
  }
  if(newSize.W == adaptee->GetWidth() && newSize.H == adaptee->GetHeight()) {
    return Napi::Boolean::New(env, true);
  }
  Robot::uint32 newLength = newSize.W * newSize.H;
  if(newLength <= adaptee->GetLimit()) {
    auto r = Napi::Boolean::New(env, adaptee->Create(newSize));
    return r;
  }
  if(!bufferRef.IsEmpty() && bufferRef.Value().IsNull()) {
    adaptee->Create(newSize);
  }
  // Only if a re-alloc of the image data would occur, and if the existing buffer was exposed, do we need to copy before resizing.
  auto image = std::make_shared<Robot::Image>(Robot::Image(*adaptee));
  if(image->Create(newSize)) {
    clear(); adaptee = image;
    return Napi::Boolean::New(env, true);
  }
  return Napi::Boolean::New(env, false);
}

void ImageAdapter::destroy(const Napi::CallbackInfo& info) {
  // Don't delegate to Robot::Image::Destroy - leave that to the destructor and ref-counting.
  // Instead just create a clean empty image.
  clear();
}

Napi::Value ImageAdapter::getWidth(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee->GetWidth());
}

Napi::Value ImageAdapter::getHeight(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee->GetHeight());
}

Napi::Value ImageAdapter::getLength(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee->GetLength());
}

// Copy the current image to new buffers because the old ones may not yet be cleared from the V8 GlobalBackingStoreRegistry
void ImageAdapter::realloc() {
  adaptee = std::make_shared<Robot::Image>(*adaptee);
  bufferRef = Napi::Reference<Napi::Value>::New(env.Global());
}
// Clear the current image
void ImageAdapter::clear() {
  adaptee = std::make_shared<Robot::Image>();
  bufferRef = Napi::Reference<Napi::Value>::New(env.Global());   // Using "global" as the erstatz object because null has special meaning, and global lives forever.
}

// As soon as an ArrayBuffer is created which references some non-null data in an Image object,
// that Image object must not be destroyed until the ArrayBuffer is torn down by V8.
// This is accomplished by using a reference-counting std::shared_ptr<Image>, and holding one of the
// references in the lambda/functor which is attached to the ArrayBuffer as a Finalizer.
// 
// see: https://github.com/nodejs/node/issues/32463
Napi::Value ImageAdapter::getData(const Napi::CallbackInfo& info) {
  // Empty images have no data.
  if(!adaptee->IsValid()) {
    return env.Null();
  }
  // Empty means stale, so re-alloc.
  if(bufferRef.IsEmpty()) {
    realloc();
  }
  // Ref-of-nonArrayBuffer means clean, so create it.
  if(!bufferRef.IsEmpty() && !bufferRef.Value().IsArrayBuffer()) {
    bufferRef = Napi::Reference<Napi::Value>::New(Napi::ArrayBuffer::New(
      env, adaptee->GetData(),
      adaptee->GetLimit() * sizeof(Robot::uint32),
      [imagePtr=adaptee](Napi::Env /*env*/, void* /*data*/) mutable {
        imagePtr.reset();
      }
    ));
  }
  // By now, the ref should hold something useful, so wrap and return it.
  return Napi::Uint32Array::New(env, adaptee->GetLength(), bufferRef.Value().As<Napi::ArrayBuffer>(), 0);
}

Napi::Value ImageAdapter::getLimit(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee->GetLimit());
}

Napi::Value ImageAdapter::getPixel(const Napi::CallbackInfo& info) {
  return ColorAdapter::New(env, adaptee->GetPixel(PointAdapter::NewAdaptee(info)));
}

void ImageAdapter::setPixel(const Napi::CallbackInfo& info) {
  if(ColorAdapter::IsInstance(info[2])) {
    adaptee->SetPixel(info[0].As<Napi::Number>().Int32Value(), info[1].As<Napi::Number>().Int32Value(), ColorAdapter::Unwrap(info[2])->adaptee);
    return;
  }
  if(ColorAdapter::IsInstance(info[1])) {
    adaptee->SetPixel(PointAdapter::NewAdaptee(env, {info[0]}), ColorAdapter::Unwrap(info[1])->adaptee);
    return;
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value ImageAdapter::fill(const Napi::CallbackInfo& info) {
  auto c = ColorAdapter::NewAdaptee(info);
  return Napi::Boolean::New(env, adaptee->Fill(c.R, c.G, c.B, c.A));
}

Napi::Value ImageAdapter::swap(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee->Swap(info[0].As<Napi::String>().Utf8Value().c_str()));
}

Napi::Value ImageAdapter::flip(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee->Flip(info[0].As<Napi::Boolean>(), info[1].As<Napi::Boolean>()));
}

Napi::Value ImageAdapter::eq(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, *adaptee == *Unwrap(info[0])->adaptee);
}

Napi::Value ImageAdapter::ne(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, *adaptee != *Unwrap(info[0])->adaptee);
}

Napi::Value ImageAdapter::toString(const Napi::CallbackInfo& info) {
  auto stringstream = std::stringstream();
  stringstream << "[" <<
      adaptee->GetWidth() << "x" <<
      adaptee->GetHeight() << " - " <<
      adaptee->GetLength() << "/" <<
      adaptee->GetLimit() << "]";
  return Napi::String::New(env, stringstream.str());
}
