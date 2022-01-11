#include "RangeAdapter.h"

Napi::Function RangeAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Range", {
    InstanceAccessor("min", &RangeAdapter::min, &RangeAdapter::min),
    InstanceAccessor("max", &RangeAdapter::max, &RangeAdapter::max),
    InstanceMethod("getRange", &RangeAdapter::getRange),
    InstanceMethod("setRange", &RangeAdapter::setRange),
    InstanceMethod("contains", &RangeAdapter::contains),
    InstanceMethod("getRandom", &RangeAdapter::getRandom),
    InstanceMethod("eq", &RangeAdapter::eq),
    InstanceMethod("ne", &RangeAdapter::ne),
  });
}

RangeAdapter::RangeAdapter(const Napi::CallbackInfo& info) : ClassAdapterEq(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Range();
    return;
  }
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    adaptee = Robot::Range(o.Get("min").ToNumber(), o.Get("max").ToNumber());
    return;
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Range(info[0].ToNumber(), info[1].ToNumber());
    return;
  } 
  adaptee = Robot::Range(info[0].ToNumber());
}

Napi::Value RangeAdapter::min(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Min);
}
void RangeAdapter::min(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Min = value.ToNumber();
}

Napi::Value RangeAdapter::max(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Max);
}
void RangeAdapter::max(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Max = value.ToNumber();
}

Napi::Value RangeAdapter::getRange(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetRange());
}

void RangeAdapter::setRange(const Napi::CallbackInfo& info) {
  adaptee = RangeAdapter(info).adaptee;
}

Napi::Value RangeAdapter::contains(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Contains(
    info[0].ToNumber(),
    info[1].IsBoolean() ? info[1].As<Napi::Boolean>() : true
  ));
}

Napi::Value RangeAdapter::getRandom(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetRandom());
}
