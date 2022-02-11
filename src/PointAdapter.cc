#include "PointAdapter.h"
#include "SizeAdapter.h"
#include <sstream>

Napi::Function PointAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Point", {
    StaticMethod("normalize", &PointAdapter::normalize),
    InstanceAccessor("x", &PointAdapter::x, &PointAdapter::x),
    InstanceAccessor("y", &PointAdapter::y, &PointAdapter::y),
    InstanceMethod("isZero", &PointAdapter::isZero),
    InstanceMethod("toSize", &PointAdapter::toSize),
    InstanceMethod("add", &PointAdapter::add),
    InstanceMethod("sub", &PointAdapter::sub),
    InstanceMethod("eq", &PointAdapter::eq),
    InstanceMethod("ne", &PointAdapter::ne),
    InstanceMethod("neg", &PointAdapter::neg),
    InstanceMethod("toString", &PointAdapter::toString),
  });
}

PointAdapter::PointAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    adaptee = Robot::Point(o.Get("x").As<Napi::Number>(), o.Get("y").As<Napi::Number>());
    return;
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Point(info[0].As<Napi::Number>(), info[1].As<Napi::Number>());
    return;
  } 
  adaptee = Robot::Point(info[0].As<Napi::Number>());
}

Napi::Value PointAdapter::normalize(const Napi::CallbackInfo& info) {
  auto o = Napi::Object::New(info.Env());
  auto p = NewAdaptee(info);
  o["x"] = p.X; o["y"] = p.Y;
  return o;
}

Napi::Value PointAdapter::x(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.X);
}
void PointAdapter::x(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.X = value.As<Napi::Number>();
}

Napi::Value PointAdapter::y(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Y);
}
void PointAdapter::y(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Y = value.As<Napi::Number>();
}

Napi::Value PointAdapter::isZero(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsZero());
}

Napi::Value PointAdapter::toSize(const Napi::CallbackInfo& info) {
  return SizeAdapter::New(env, adaptee.ToSize());
}

Napi::Value PointAdapter::add(const Napi::CallbackInfo& info) {
  return New(env, adaptee + NewAdaptee(info));
}

Napi::Value PointAdapter::sub(const Napi::CallbackInfo& info) {
  return New(env, adaptee - NewAdaptee(info));
}

Napi::Value PointAdapter::neg(const Napi::CallbackInfo& info) {
  return New(env, -adaptee);
}

Napi::Value PointAdapter::toString(const Napi::CallbackInfo& info) {
  auto stringstream = std::stringstream();
  stringstream << "[" <<
      adaptee.X << ", " <<
      adaptee.Y << "]";
  return Napi::String::New(env, stringstream.str());
}
