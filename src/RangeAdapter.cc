#include "RangeAdapter.h"
#include <sstream>

Napi::Function RangeAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Range", {
    StaticMethod("normalize", &RangeAdapter::normalize),
    InstanceAccessor("min", &RangeAdapter::min, &RangeAdapter::min),
    InstanceAccessor("max", &RangeAdapter::max, &RangeAdapter::max),
    InstanceMethod("getRange", &RangeAdapter::getRange),
    InstanceMethod("setRange", &RangeAdapter::setRange),
    InstanceMethod("contains", &RangeAdapter::contains),
    InstanceMethod("getRandom", &RangeAdapter::getRandom),
    InstanceMethod("eq", &RangeAdapter::eq),
    InstanceMethod("ne", &RangeAdapter::ne),
    InstanceMethod("toString", &RangeAdapter::toString),
  });
}

RangeAdapter::RangeAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    adaptee = Robot::Range(o.Get("min").As<Napi::Number>(), o.Get("max").As<Napi::Number>());
    return;
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Range(info[0].As<Napi::Number>(), info[1].As<Napi::Number>());
    return;
  } 
  adaptee = Robot::Range(info[0].As<Napi::Number>());
}

Napi::Value RangeAdapter::normalize(const Napi::CallbackInfo& info) {
  auto o = Napi::Object::New(info.Env());
  auto r = NewAdaptee(info);
  o["min"] = r.Min; o["max"] = r.Max;
  return o;
}

Napi::Value RangeAdapter::min(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Min);
}
void RangeAdapter::min(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Min = value.As<Napi::Number>();
}

Napi::Value RangeAdapter::max(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Max);
}
void RangeAdapter::max(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Max = value.As<Napi::Number>();
}

Napi::Value RangeAdapter::getRange(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetRange());
}

void RangeAdapter::setRange(const Napi::CallbackInfo& info) {
  adaptee = NewAdaptee(info);
}

Napi::Value RangeAdapter::contains(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Contains(
    info[0].As<Napi::Number>(),
    info[1].IsUndefined() ? true : info[1].As<Napi::Boolean>()
  ));
}

Napi::Value RangeAdapter::getRandom(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetRandom());
}

Napi::Value RangeAdapter::toString(const Napi::CallbackInfo& info) {
  auto stringstream = std::stringstream();
  stringstream << "[" <<
      adaptee.Min << ", " <<
      adaptee.Max << "]";
  return Napi::String::New(env, stringstream.str());
}
