#include "ScreenAdapter.h"
#include "BoundsAdapter.h"
#include "PointAdapter.h"
#include "ImageAdapter.h"
#include "WindowAdapter.h"

Napi::Function ScreenAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Screen", {
    InstanceMethod("getUsable", &ScreenAdapter::getUsable),
    InstanceMethod("getBounds", &ScreenAdapter::getBounds),
    InstanceMethod("isPortrait", &ScreenAdapter::isPortrait),
    InstanceMethod("isLandscape", &ScreenAdapter::isLandscape),
    StaticMethod("synchronize", &ScreenAdapter::synchronize),
    StaticMethod("getMain", &ScreenAdapter::getMain),
    StaticMethod("getList", &ScreenAdapter::getList),
    StaticMethod("getScreen", &ScreenAdapter::getScreen),
    StaticMethod("grabScreen", &ScreenAdapter::grabScreen),
    StaticMethod("getTotalBounds", &ScreenAdapter::getTotalBounds),
    StaticMethod("getTotalUsable", &ScreenAdapter::getTotalUsable),
    StaticMethod("isCompositing", &ScreenAdapter::isCompositing),
    StaticMethod("setCompositing", &ScreenAdapter::setCompositing),
  });
}

ScreenAdapter::ScreenAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Screen();
    return;
  }
  if(BoundsAdapter::IsInstance(info[0]) && BoundsAdapter::IsInstance(info[1])) {
    adaptee = Robot::Screen(BoundsAdapter::Unwrap(info[0])->adaptee, BoundsAdapter::Unwrap(info[1])->adaptee);
    return;
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value ScreenAdapter::getUsable(const Napi::CallbackInfo& info) {
  return BoundsAdapter::New(env, adaptee.GetUsable());
}

Napi::Value ScreenAdapter::getBounds(const Napi::CallbackInfo& info) {
  return BoundsAdapter::New(env, adaptee.GetBounds());
}

Napi::Value ScreenAdapter::isPortrait(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsPortrait());
}

Napi::Value ScreenAdapter::isLandscape(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsLandscape());
}

Napi::Value ScreenAdapter::synchronize(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), Robot::Screen::Synchronize());
}

Napi::Value ScreenAdapter::getMain(const Napi::CallbackInfo& info) {
  return New(info.Env(), *Robot::Screen::GetMain());
}

Napi::Value ScreenAdapter::getList(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  auto screens = Robot::Screen::GetList();
  auto arr = Napi::Array::New(env, screens.size());
  for(size_t i = 0; i < screens.size(); i++) {
    arr[i] = New(env, *screens[i]);
  }
  return arr;
}

Napi::Value ScreenAdapter::getScreen(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  if(WindowAdapter::IsInstance(info[0])) {
    return New(env, *Robot::Screen::GetScreen(WindowAdapter::Unwrap(info[0])->adaptee));
  }
  return New(env, *Robot::Screen::GetScreen(PointAdapter(info).adaptee));
}

Napi::Value ScreenAdapter::grabScreen(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  if(!ImageAdapter::IsInstance(info[0])) {
    throw Napi::TypeError::New(env, "Invalid arguments");
  }
  return Napi::Boolean::New(env, Robot::Screen::GrabScreen(
      ImageAdapter::Unwrap(info[0])->adaptee,
      BoundsAdapter::IsInstance(info[1]) ?
        BoundsAdapter::Unwrap(info[1])->adaptee :
        BoundsAdapter::Unwrap(BoundsAdapter::GetConstructor(env).New({info[1], info[2], info[3], info[4]}))->adaptee,
      info[2].IsUndefined() ? Robot::Window() : WindowAdapter::Unwrap(info[2])->adaptee
  ));
}

Napi::Value ScreenAdapter::getTotalUsable(const Napi::CallbackInfo& info) {
  return BoundsAdapter::New(info.Env(), Robot::Screen::GetTotalUsable());
}

Napi::Value ScreenAdapter::getTotalBounds(const Napi::CallbackInfo& info) {
  return BoundsAdapter::New(info.Env(), Robot::Screen::GetTotalBounds());
}

Napi::Value ScreenAdapter::isCompositing(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), Robot::Screen::IsCompositing());
}

void ScreenAdapter::setCompositing(const Napi::CallbackInfo& info) {
  Robot::Screen::SetCompositing(info[0].ToBoolean());
}
