#include "PointAdapter.h"
#include "SizeAdapter.h"

Napi::Function PointAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Point", {
    InstanceAccessor("x", &PointAdapter::x, &PointAdapter::x),
    InstanceAccessor("y", &PointAdapter::y, &PointAdapter::y),
    InstanceMethod("isZero", &PointAdapter::isZero),
    InstanceMethod("toSize", &PointAdapter::toSize),
    InstanceMethod("add", &PointAdapter::add),
    InstanceMethod("sub", &PointAdapter::sub),
    InstanceMethod("eq", &PointAdapter::eq),
    InstanceMethod("ne", &PointAdapter::ne),
    InstanceMethod("neg", &PointAdapter::neg),
  });
}

PointAdapter::PointAdapter(const Napi::CallbackInfo& info) : ClassAdapterEq(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Point();
    return;
  }
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    adaptee = Robot::Point(o.Get("x").ToNumber(), o.Get("y").ToNumber());
    return;
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Point(info[0].ToNumber(), info[1].ToNumber());
    return;
  } 
  adaptee = Robot::Point(info[0].ToNumber());
}

Napi::Value PointAdapter::x(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.X);
}
void PointAdapter::x(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.X = value.ToNumber();
}

Napi::Value PointAdapter::y(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Y);
}
void PointAdapter::y(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Y = value.ToNumber();
}

Napi::Value PointAdapter::isZero(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsZero());
}

Napi::Value PointAdapter::toSize(const Napi::CallbackInfo& info) {
  return SizeAdapter::New(env, adaptee.ToSize());
}

Napi::Value PointAdapter::add(const Napi::CallbackInfo& info) {
  return New(env, adaptee + PointAdapter(info).adaptee);
}

Napi::Value PointAdapter::sub(const Napi::CallbackInfo& info) {
  return New(env, adaptee - PointAdapter(info).adaptee);
}

Napi::Value PointAdapter::neg(const Napi::CallbackInfo& info) {
  return New(env, -adaptee);
}
