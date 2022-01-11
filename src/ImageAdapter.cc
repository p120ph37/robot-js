#include "ImageAdapter.h"
#include "ColorAdapter.h"
#include "PointAdapter.h"
#include "SizeAdapter.h"

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
  });
}

ImageAdapter::ImageAdapter(const Napi::CallbackInfo& info) : ClassAdapterEq(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Image();
    return;
  }
  adaptee = Robot::Image(SizeAdapter(info).adaptee);
}

Napi::Value ImageAdapter::isValid(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsValid());
}

Napi::Value ImageAdapter::create(const Napi::CallbackInfo& info) {
  return Napi::Boolean::From(env, adaptee.Create(SizeAdapter(info).adaptee));
}

void ImageAdapter::destroy(const Napi::CallbackInfo& info) {
  adaptee.Destroy();
}

Napi::Value ImageAdapter::getWidth(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetWidth());
}

Napi::Value ImageAdapter::getHeight(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetHeight());
}

Napi::Value ImageAdapter::getLength(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetLength());
}

Napi::Value ImageAdapter::getData(const Napi::CallbackInfo& info) {
  if(!adaptee.IsValid()) return env.Null();
  auto data = adaptee.GetData();
  auto length = adaptee.GetLength();
  return Napi::Uint32Array::New(env, length, Napi::ArrayBuffer::New(env, data, length * sizeof(*data)), 0);
}

Napi::Value ImageAdapter::getLimit(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetLimit());
}

Napi::Value ImageAdapter::getPixel(const Napi::CallbackInfo& info) {
  return ColorAdapter::New(env, adaptee.GetPixel(PointAdapter(info).adaptee));
}

void ImageAdapter::setPixel(const Napi::CallbackInfo& info) {
  if(PointAdapter::IsInstance(info[0]) && ColorAdapter::IsInstance(info[1])) {
    adaptee.SetPixel(PointAdapter::Unwrap(info[0])->adaptee, ColorAdapter::Unwrap(info[1])->adaptee);
    return;
  }
  if(ColorAdapter::IsInstance(info[2])) {
    adaptee.SetPixel(info[0].ToNumber().Int32Value(), info[1].ToNumber().Int32Value(), ColorAdapter::Unwrap(info[2])->adaptee);
    return;
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value ImageAdapter::fill(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Fill(ColorAdapter(info).adaptee));
}

Napi::Value ImageAdapter::swap(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Swap(info[0].ToString().Utf8Value().c_str()));
}

Napi::Value ImageAdapter::flip(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Flip(info[0].ToBoolean(), info[1].ToBoolean()));
}
