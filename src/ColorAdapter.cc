#include "ColorAdapter.h"

Napi::Function ColorAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Color", {
    InstanceAccessor("a", &ColorAdapter::a, &ColorAdapter::a),
    InstanceAccessor("r", &ColorAdapter::r, &ColorAdapter::r),
    InstanceAccessor("g", &ColorAdapter::g, &ColorAdapter::g),
    InstanceAccessor("b", &ColorAdapter::b, &ColorAdapter::b),
    InstanceMethod("getARGB", &ColorAdapter::getARGB),
    InstanceMethod("setARGB", &ColorAdapter::setARGB),
    InstanceMethod("eq", &ColorAdapter::eq),
    InstanceMethod("ne", &ColorAdapter::ne),
  });
}

ColorAdapter::ColorAdapter(const Napi::CallbackInfo& info) : ClassAdapterEq(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Color();
    return;
  }
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    adaptee = Robot::Color(
      o.Get("r").ToNumber().Int32Value(),
      o.Get("g").ToNumber().Int32Value(),
      o.Get("b").ToNumber().Int32Value(),
      o.Get("a").IsUndefined() ? 255 : o.Get("a").ToNumber().Int32Value()
    );
    return;
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Color(
      info[0].ToNumber().Int32Value(),
      info[1].ToNumber().Int32Value(),
      info[2].ToNumber().Int32Value(),
      info[3].IsUndefined() ? 255 : info[3].ToNumber().Int32Value()
    );
    return;
  }
  adaptee = Robot::Color(info[0].ToNumber().Int32Value());
}

Napi::Value ColorAdapter::a(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.A);
}
void ColorAdapter::a(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.A = value.ToNumber().Int32Value();
}

Napi::Value ColorAdapter::r(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.R);
}
void ColorAdapter::r(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.R = value.ToNumber().Int32Value();
}

Napi::Value ColorAdapter::g(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.G);
}
void ColorAdapter::g(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.G = value.ToNumber().Int32Value();
}

Napi::Value ColorAdapter::b(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.B);
}
void ColorAdapter::b(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.B = value.ToNumber().Int32Value();
}

Napi::Value ColorAdapter::getARGB(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetARGB());
}
void ColorAdapter::setARGB(const Napi::CallbackInfo& info) {
  adaptee.SetARGB(info[0].ToNumber());
}
