#include "RobotAdapter.h"
#include "ImageAdapter.h"
#include "ClipboardAdapter.h"

Napi::Object ClipboardAdapter::Init(Napi::Env env) {
  auto obj(Napi::Object::New(env));
  obj["clear"] = Napi::Function::New<clear>(env);
  obj["hasText"] = Napi::Function::New<hasText>(env);
  obj["getText"] = Napi::Function::New<getText>(env);
  obj["setText"] = Napi::Function::New<setText>(env);
  obj["hasImage"] = Napi::Function::New<hasImage>(env);
  obj["getImage"] = Napi::Function::New<getImage>(env);
  obj["setImage"] = Napi::Function::New<setImage>(env);
  obj["getSequence"] = Napi::Function::New<getSequence>(env);
  return obj;
}

Napi::Value ClipboardAdapter::clear(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), Robot::Clipboard::Clear());
}

Napi::Value ClipboardAdapter::hasText(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), Robot::Clipboard::HasText());
}

Napi::Value ClipboardAdapter::getText(const Napi::CallbackInfo& info) {
  return Napi::String::New(info.Env(), Robot::Clipboard::GetText().data());
}

Napi::Value ClipboardAdapter::setText(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  if(!info[0].IsString()) {
    throw Napi::TypeError::New(env, "Invalid arguments");
  }
  return Napi::Boolean::New(env, Robot::Clipboard::SetText(
    info[0].As<Napi::String>().Utf8Value().c_str()
  ));
}

Napi::Value ClipboardAdapter::hasImage(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), Robot::Clipboard::HasImage());
}

Napi::Value ClipboardAdapter::getImage(const Napi::CallbackInfo& info) {
  ImageAdapter::Unwrap(info[0])->destroy(info); // Can't reuse the underlying buffers due to V8 stuff.
  return Napi::Boolean::New(info.Env(), Robot::Clipboard::GetImage(
    *ImageAdapter::Unwrap(info[0])->adaptee.get()
  ));
}

Napi::Value ClipboardAdapter::setImage(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), Robot::Clipboard::SetImage(
    *ImageAdapter::Unwrap(info[0])->adaptee.get()
  ));
}

Napi::Value ClipboardAdapter::getSequence(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), Robot::Clipboard::GetSequence());
}
