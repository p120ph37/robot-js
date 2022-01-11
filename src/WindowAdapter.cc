#include "WindowAdapter.h"
#include "BoundsAdapter.h"
#include "PointAdapter.h"
#include "ImageAdapter.h"
#include "ProcessAdapter.h"

Napi::Function WindowAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Window", {
    InstanceMethod("isValid", &WindowAdapter::isValid),
    InstanceMethod("close", &WindowAdapter::close),
    InstanceMethod("isTopMost", &WindowAdapter::isTopMost),
    InstanceMethod("isBorderless", &WindowAdapter::isBorderless),
    InstanceMethod("isMinimized", &WindowAdapter::isMinimized),
    InstanceMethod("isMaximized", &WindowAdapter::isMaximized),
    InstanceMethod("getProcess", &WindowAdapter::getProcess),
    InstanceMethod("getPID", &WindowAdapter::getPID),
    InstanceMethod("getHandle", &WindowAdapter::getHandle),
    InstanceMethod("setHandle", &WindowAdapter::setHandle),
    InstanceMethod("getTitle", &WindowAdapter::getTitle),
    InstanceMethod("setTitle", &WindowAdapter::setTitle),
    InstanceMethod("getBounds", &WindowAdapter::getBounds),
    InstanceMethod("setBounds", &WindowAdapter::setBounds),
    InstanceMethod("getClient", &WindowAdapter::getClient),
    InstanceMethod("setClient", &WindowAdapter::setClient),
    InstanceMethod("mapToClient", &WindowAdapter::mapToClient),
    InstanceMethod("mapToScreen", &WindowAdapter::mapToScreen),
    StaticMethod("getList", &WindowAdapter::getList),
    StaticMethod("getActive", &WindowAdapter::getActive),
    StaticMethod("setActive", &WindowAdapter::setActive),
    StaticMethod("isAxEnabled", &WindowAdapter::isAxEnabled),
  });
}

WindowAdapter::WindowAdapter(const Napi::CallbackInfo& info) : ClassAdapterEq(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Window();
    return;
  }
  adaptee = Robot::Window((Robot::uintptr)info[0].ToNumber().Int64Value());
}

Napi::Value WindowAdapter::isValid(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsValid());
}

void WindowAdapter::close(const Napi::CallbackInfo& info) {
  adaptee.Close();
}

Napi::Value WindowAdapter::isTopMost(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsTopMost());
}

Napi::Value WindowAdapter::isBorderless(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsBorderless());
}

Napi::Value WindowAdapter::isMinimized(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsMinimized());
}

Napi::Value WindowAdapter::isMaximized(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsMaximized());
}

Napi::Value WindowAdapter::getProcess(const Napi::CallbackInfo& info) {
  return ProcessAdapter::New(env, adaptee.GetProcess());
}

Napi::Value WindowAdapter::getPID(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetPID());
}

Napi::Value WindowAdapter::getHandle(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetHandle());
}

Napi::Value WindowAdapter::setHandle(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.SetHandle(info[0].ToNumber().Int64Value()));
}

Napi::Value WindowAdapter::getTitle(const Napi::CallbackInfo& info) {
  return Napi::String::New(env, adaptee.GetTitle());
}

void WindowAdapter::setTitle(const Napi::CallbackInfo& info) {
  adaptee.SetTitle(info[0].ToString().Utf8Value().c_str());
}

Napi::Value WindowAdapter::getBounds(const Napi::CallbackInfo& info) {
  return BoundsAdapter::New(env, adaptee.GetBounds());
}

void WindowAdapter::setBounds(const Napi::CallbackInfo& info) {
  adaptee.SetBounds(BoundsAdapter(info).adaptee);
}

Napi::Value WindowAdapter::getClient(const Napi::CallbackInfo& info) {
  return BoundsAdapter::New(env, adaptee.GetClient());
}

void WindowAdapter::setClient(const Napi::CallbackInfo& info) {
  adaptee.SetClient(BoundsAdapter(info).adaptee);
}

Napi::Value WindowAdapter::mapToClient(const Napi::CallbackInfo& info) {
  return PointAdapter::New(env, adaptee.MapToClient(PointAdapter(info).adaptee));
}

Napi::Value WindowAdapter::mapToScreen(const Napi::CallbackInfo& info) {
  return PointAdapter::New(env, adaptee.MapToScreen(PointAdapter(info).adaptee));
}

Napi::Value WindowAdapter::getList(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  auto windows = Robot::Window::GetList();
  auto arr = Napi::Array::New(env, windows.size());
  for(size_t i = 0; i < windows.size(); i++) {
    arr[i] = New(env, windows[i]);
  }
  return arr;
}

Napi::Value WindowAdapter::getActive(const Napi::CallbackInfo& info) {
  return New(info.Env(), Robot::Window::GetActive());
}

void WindowAdapter::setActive(const Napi::CallbackInfo& info) {
  if(IsInstance(info[0])) {
    Robot::Window::SetActive(Unwrap(info[0])->adaptee);
    return;
  }
  throw Napi::TypeError::New(info.Env(), "Invalid arguments");
}

Napi::Value WindowAdapter::isAxEnabled(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), Robot::Window::IsAxEnabled());
}
