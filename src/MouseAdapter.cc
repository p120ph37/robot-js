#include "MouseAdapter.h"
#include "RangeAdapter.h"
#include "PointAdapter.h"

Napi::Function MouseAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Mouse", {
    InstanceAccessor("autoDelay", &MouseAdapter::autoDelay, &MouseAdapter::autoDelay),
    InstanceMethod("click", &MouseAdapter::click),
    InstanceMethod("press", &MouseAdapter::press),
    InstanceMethod("release", &MouseAdapter::release),
    InstanceMethod("scrollH", &MouseAdapter::scrollH),
    InstanceMethod("scrollV", &MouseAdapter::scrollV),
    StaticMethod("getPos", &MouseAdapter::getPos),
    StaticMethod("setPos", &MouseAdapter::setPos),
    StaticMethod("getState", &MouseAdapter::getState),
  });
}

MouseAdapter::MouseAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value MouseAdapter::autoDelay(const Napi::CallbackInfo& info) {
  return RangeAdapter::New(env, adaptee.AutoDelay);
}
void MouseAdapter::autoDelay(const Napi::CallbackInfo& info, const Napi::Value& value) {
  if(RangeAdapter::IsInstance(info[0])) {
    adaptee.AutoDelay = RangeAdapter::Unwrap(info[0])->adaptee;
    return;
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}

void MouseAdapter::click(const Napi::CallbackInfo& info) {
  adaptee.Click((Robot::Button)info[0].As<Napi::Number>().Int32Value());
}

void MouseAdapter::press(const Napi::CallbackInfo& info) {
  adaptee.Press((Robot::Button)info[0].As<Napi::Number>().Int32Value());
}

void MouseAdapter::release(const Napi::CallbackInfo& info) {
  adaptee.Release((Robot::Button)info[0].As<Napi::Number>().Int32Value());
}

void MouseAdapter::scrollH(const Napi::CallbackInfo& info) {
  adaptee.ScrollH(info[0].As<Napi::Number>());
}

void MouseAdapter::scrollV(const Napi::CallbackInfo& info) {
  adaptee.ScrollV(info[0].As<Napi::Number>());
}

Napi::Value MouseAdapter::getPos(const Napi::CallbackInfo& info) {
  return PointAdapter::New(info.Env(), Robot::Mouse::GetPos());
}

void MouseAdapter::setPos(const Napi::CallbackInfo& info) {
  Robot::Mouse::SetPos(PointAdapter::NewAdaptee(info));
}

Napi::Value MouseAdapter::getState(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  if(info[0].IsUndefined()) {
    auto o = Napi::Object::New(env);
    Robot::ButtonState state;
    if(Robot::Mouse::GetState(state)) {
      for(auto el : state) {
        o.Set(el.first, Napi::Boolean::New(env, el.second));
      }
    }
    return o;
  }
  return Napi::Boolean::New(env, Robot::Mouse::GetState((Robot::Button)info[0].As<Napi::Number>().Int32Value()));
}
