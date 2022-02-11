#include "ColorAdapter.h"
#include <sstream>

Napi::Function ColorAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Color", {
    StaticMethod("normalize", &ColorAdapter::normalize),
    InstanceAccessor("a", &ColorAdapter::a, &ColorAdapter::a),
    InstanceAccessor("r", &ColorAdapter::r, &ColorAdapter::r),
    InstanceAccessor("g", &ColorAdapter::g, &ColorAdapter::g),
    InstanceAccessor("b", &ColorAdapter::b, &ColorAdapter::b),
    InstanceMethod("getARGB", &ColorAdapter::getARGB),
    InstanceMethod("setARGB", &ColorAdapter::setARGB),
    InstanceMethod("eq", &ColorAdapter::eq),
    InstanceMethod("ne", &ColorAdapter::ne),
    InstanceMethod("toString", &ColorAdapter::toString),
  });
}

ColorAdapter::ColorAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    adaptee = Robot::Color(
      o.Get("r").As<Napi::Number>().Int32Value(),
      o.Get("g").As<Napi::Number>().Int32Value(),
      o.Get("b").As<Napi::Number>().Int32Value(),
      o.Get("a").IsUndefined() ? 255 : o.Get("a").As<Napi::Number>().Int32Value()
    );
    return;
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Color(
      info[0].As<Napi::Number>().Int32Value(),
      info[1].As<Napi::Number>().Int32Value(),
      info[2].As<Napi::Number>().Int32Value(),
      info[3].IsUndefined() ? 255 : info[3].As<Napi::Number>().Int32Value()
    );
    return;
  }
  adaptee = Robot::Color(info[0].As<Napi::Number>().Int32Value());
}

Napi::Value ColorAdapter::normalize(const Napi::CallbackInfo& info) {
  auto o = Napi::Object::New(info.Env());
  auto c = NewAdaptee(info);
  o["r"] = c.R; o["g"] = c.G; o["b"] = c.B; o["a"] = c.A;
  return o;
}

Napi::Value ColorAdapter::a(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.A);
}
void ColorAdapter::a(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.A = value.As<Napi::Number>().Int32Value();
}

Napi::Value ColorAdapter::r(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.R);
}
void ColorAdapter::r(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.R = value.As<Napi::Number>().Int32Value();
}

Napi::Value ColorAdapter::g(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.G);
}
void ColorAdapter::g(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.G = value.As<Napi::Number>().Int32Value();
}

Napi::Value ColorAdapter::b(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.B);
}
void ColorAdapter::b(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.B = value.As<Napi::Number>().Int32Value();
}

Napi::Value ColorAdapter::getARGB(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetARGB());
}
void ColorAdapter::setARGB(const Napi::CallbackInfo& info) {
  adaptee.SetARGB(info[0].As<Napi::Number>());
}

Napi::Value ColorAdapter::toString(const Napi::CallbackInfo& info) {
  auto stringstream = std::stringstream();
  stringstream << "[" <<
      adaptee.R << ", " <<
      adaptee.G << ", " <<
      adaptee.B << ", " <<
      adaptee.A << "]";
  return Napi::String::New(env, stringstream.str());
}
