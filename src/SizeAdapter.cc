#include "PointAdapter.h"
#include "SizeAdapter.h"

Napi::Function SizeAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Size", {
    InstanceAccessor("w", &SizeAdapter::w, &SizeAdapter::w),
    InstanceAccessor("h", &SizeAdapter::h, &SizeAdapter::h),
    InstanceMethod("isZero", &SizeAdapter::isZero),
    InstanceMethod("toPoint", &SizeAdapter::toPoint),
    InstanceMethod("add", &SizeAdapter::add),
    InstanceMethod("sub", &SizeAdapter::sub),
    InstanceMethod("eq", &SizeAdapter::eq),
    InstanceMethod("ne", &SizeAdapter::ne),
  });
}

SizeAdapter::SizeAdapter(const Napi::CallbackInfo& info) : ClassAdapterEq(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Size();
    return;
  }
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    adaptee = Robot::Size(o.Get("w").ToNumber(), o.Get("h").ToNumber());
    return;
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Size(info[0].ToNumber(), info[1].ToNumber());
    return;
  } 
  adaptee = Robot::Size(info[0].ToNumber());
}

Napi::Value SizeAdapter::w(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.W);
}
void SizeAdapter::w(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.W = value.ToNumber();
}

Napi::Value SizeAdapter::h(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.H);
}
void SizeAdapter::h(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.H = value.ToNumber();
}

Napi::Value SizeAdapter::isZero(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsZero());
}

Napi::Value SizeAdapter::toPoint(const Napi::CallbackInfo& info) {
  return PointAdapter::New(env, adaptee.ToPoint());
}

Napi::Value SizeAdapter::add(const Napi::CallbackInfo& info) {
  return New(env, adaptee + SizeAdapter(info).adaptee);
}

Napi::Value SizeAdapter::sub(const Napi::CallbackInfo& info) {
  return New(env, adaptee - SizeAdapter(info).adaptee);
}
