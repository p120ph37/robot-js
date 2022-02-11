#include "ScreenAdapter.h"
#include "BoundsAdapter.h"
#include "PointAdapter.h"
#include "ImageAdapter.h"
#include "WindowAdapter.h"

namespace {
  const char* SCREENS = "screens";
}

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
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
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
  auto env = info.Env();
  if(Robot::Screen::Synchronize() == false) {
    return Napi::Boolean::New(env, false);
  }
  auto screens = Robot::Screen::GetList();
  auto arr = Napi::Array::New(env, screens.size());
  for(size_t i = 0; i < screens.size(); i++) {
    arr[i] = New(env, *screens[i]);
  }
  EnvLocal(env)[SCREENS] = arr;
  return Napi::Boolean::New(env, true);
}

Napi::Value ScreenAdapter::getMain(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  if(EnvLocal(env).Has(SCREENS)) {
    return EnvLocal(env).Get(SCREENS).As<Napi::Array>()[0U];
  }
  return env.Null();
}

Napi::Value ScreenAdapter::getList(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  if(EnvLocal(env).Has(SCREENS)) {
    return EnvLocal(env).Get(SCREENS).As<Napi::Array>();
  }
  return Napi::Array::New(env, 0);
}

Napi::Value ScreenAdapter::getScreen(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  Robot::Point p;
  if(WindowAdapter::IsInstance(info[0])) {
    auto w = WindowAdapter::Unwrap(info[0])->adaptee;
    if(w.IsValid()) {
      p = w.GetBounds().GetCenter();
    } else {
      return env.Null();
    }
  } else {
    p = PointAdapter::NewAdaptee(info);
  }
  if(EnvLocal(env).Has(SCREENS)) {
    for(auto el : EnvLocal(env).Get(SCREENS).As<Napi::Array>()) {
      if(ScreenAdapter::Unwrap(el.second)->adaptee.GetBounds().Contains(p)) {
        return el.second;
      }
    }
  }
  return getMain(info);
}

Napi::Value ScreenAdapter::grabScreen(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  if(!ImageAdapter::IsInstance(info[0])) {
    throw Napi::TypeError::New(env, "Invalid arguments");
  }
  ImageAdapter::Unwrap(info[0])->destroy(info); // Can't reuse the underlying buffers due to V8 stuff.
  return Napi::Boolean::New(env, BoundsAdapter::IsInstance(info[1]) ?
    Robot::Screen::GrabScreen(
      *ImageAdapter::Unwrap(info[0])->adaptee.get(),
      BoundsAdapter::Unwrap(info[1])->adaptee,
      info[2].IsUndefined() ? Robot::Window() : WindowAdapter::Unwrap(info[2])->adaptee
    ) :
    Robot::Screen::GrabScreen(
      *ImageAdapter::Unwrap(info[0])->adaptee.get(),
      BoundsAdapter::NewAdaptee(env, {info[1], info[2], info[3], info[4]}),
      info[5].IsUndefined() ? Robot::Window() : WindowAdapter::Unwrap(info[5])->adaptee
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
  Robot::Screen::SetCompositing(info[0].As<Napi::Boolean>());
}
