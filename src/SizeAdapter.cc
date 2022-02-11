#include "PointAdapter.h"
#include "SizeAdapter.h"
#include <sstream>

Napi::Function SizeAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Size", {
    StaticMethod("normalize", &SizeAdapter::normalize),
    InstanceAccessor("w", &SizeAdapter::w, &SizeAdapter::w),
    InstanceAccessor("h", &SizeAdapter::h, &SizeAdapter::h),
    InstanceMethod("isZero", &SizeAdapter::isZero),
    InstanceMethod("isEmpty", &SizeAdapter::isEmpty),
    InstanceMethod("toPoint", &SizeAdapter::toPoint),
    InstanceMethod("add", &SizeAdapter::add),
    InstanceMethod("sub", &SizeAdapter::sub),
    InstanceMethod("eq", &SizeAdapter::eq),
    InstanceMethod("ne", &SizeAdapter::ne),
    InstanceMethod("toString", &SizeAdapter::toString),
  });
}

SizeAdapter::SizeAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    adaptee = Robot::Size(o.Get("w").As<Napi::Number>(), o.Get("h").As<Napi::Number>());
    return;
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Size(info[0].As<Napi::Number>(), info[1].As<Napi::Number>());
    return;
  } 
  adaptee = Robot::Size(info[0].As<Napi::Number>());
}

Napi::Value SizeAdapter::normalize(const Napi::CallbackInfo& info) {
  auto o = Napi::Object::New(info.Env());
  auto s = NewAdaptee(info);
  o["w"] = s.W; o["h"] = s.H;
  return o;
}

Napi::Value SizeAdapter::w(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.W);
}
void SizeAdapter::w(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.W = value.As<Napi::Number>();
}

Napi::Value SizeAdapter::h(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.H);
}
void SizeAdapter::h(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.H = value.As<Napi::Number>();
}

Napi::Value SizeAdapter::isZero(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsZero());
}

Napi::Value SizeAdapter::isEmpty(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsEmpty());
}

Napi::Value SizeAdapter::toPoint(const Napi::CallbackInfo& info) {
  return PointAdapter::New(env, adaptee.ToPoint());
}

Napi::Value SizeAdapter::add(const Napi::CallbackInfo& info) {
  return New(env, adaptee + NewAdaptee(info));
}

Napi::Value SizeAdapter::sub(const Napi::CallbackInfo& info) {
  return New(env, adaptee - NewAdaptee(info));
}

Napi::Value SizeAdapter::toString(const Napi::CallbackInfo& info) {
  auto stringstream = std::stringstream();
  stringstream << "[" <<
      adaptee.W << ", " <<
      adaptee.H << "]";
  return Napi::String::New(env, stringstream.str());
}
