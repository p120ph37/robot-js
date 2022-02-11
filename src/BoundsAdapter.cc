#include "BoundsAdapter.h"
#include "PointAdapter.h"
#include "SizeAdapter.h"
#include <sstream>

Napi::Function BoundsAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Bounds", {
    StaticMethod("normalize", &BoundsAdapter::normalize),
    InstanceAccessor("x", &BoundsAdapter::x, &BoundsAdapter::x),
    InstanceAccessor("y", &BoundsAdapter::y, &BoundsAdapter::y),
    InstanceAccessor("w", &BoundsAdapter::w, &BoundsAdapter::w),
    InstanceAccessor("h", &BoundsAdapter::h, &BoundsAdapter::h),
    InstanceMethod("isZero", &BoundsAdapter::isZero),
    InstanceMethod("isEmpty", &BoundsAdapter::isEmpty),
    InstanceMethod("isValid", &BoundsAdapter::isValid),
    InstanceMethod("getLeft", &BoundsAdapter::getLeft),
    InstanceMethod("getRight", &BoundsAdapter::getRight),
    InstanceMethod("getTop", &BoundsAdapter::getTop),
    InstanceMethod("getBottom", &BoundsAdapter::getBottom),
    InstanceMethod("setLeft", &BoundsAdapter::setLeft),
    InstanceMethod("setRight", &BoundsAdapter::setRight),
    InstanceMethod("setTop", &BoundsAdapter::setTop),
    InstanceMethod("setBottom", &BoundsAdapter::setBottom),
    InstanceMethod("getLTRB", &BoundsAdapter::getLTRB),
    InstanceMethod("setLTRB", &BoundsAdapter::setLTRB),
    InstanceMethod("normalize", &BoundsAdapter::normalizeSelf),
    InstanceMethod("containsP", &BoundsAdapter::containsP),
    InstanceMethod("containsB", &BoundsAdapter::containsB),
    InstanceMethod("intersects", &BoundsAdapter::intersects),
    InstanceMethod("getPoint", &BoundsAdapter::getPoint),
    InstanceMethod("setPoint", &BoundsAdapter::setPoint),
    InstanceMethod("getSize", &BoundsAdapter::getSize),
    InstanceMethod("setSize", &BoundsAdapter::setSize),
    InstanceMethod("getCenter", &BoundsAdapter::getCenter),
    InstanceMethod("unite", &BoundsAdapter::unite),
    InstanceMethod("intersect", &BoundsAdapter::intersect),
    InstanceMethod("eq", &BoundsAdapter::eq),
    InstanceMethod("ne", &BoundsAdapter::ne),
    InstanceMethod("toString", &BoundsAdapter::toString),
  });
}

BoundsAdapter::BoundsAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  if(PointAdapter::IsInstance(info[0]) && SizeAdapter::IsInstance(info[1])) {
    adaptee = Robot::Bounds(PointAdapter::Unwrap(info[0])->adaptee, SizeAdapter::Unwrap(info[1])->adaptee);
    return;
  }
  if(info[0].IsObject() && info[1].IsObject()) {
    adaptee = Robot::Bounds(PointAdapter::NewAdaptee(env, {info[0]}), SizeAdapter::NewAdaptee(env, {info[1]}));
    return;
  }
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    if(o.Has("x") && o.Has("y") && o.Has("w") && o.Has("h")) {
      adaptee = Robot::Bounds(o.Get("x").As<Napi::Number>(), o.Get("y").As<Napi::Number>(), o.Get("w").As<Napi::Number>(), o.Get("h").As<Napi::Number>());
      return;
    }
    if(o.Has("l") && o.Has("t") && o.Has("r") && o.Has("b")) {
      adaptee = Robot::Bounds();
      adaptee.SetLTRB(o.Get("l").As<Napi::Number>(), o.Get("t").As<Napi::Number>(), o.Get("r").As<Napi::Number>(), o.Get("b").As<Napi::Number>());
      return;
    }
  }
  if(!info[2].IsUndefined()) {
    adaptee = Robot::Bounds(info[0].As<Napi::Number>(), info[1].As<Napi::Number>(), info[2].As<Napi::Number>(), info[3].As<Napi::Number>());
    return;
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Bounds(info[0].As<Napi::Number>(), info[0].As<Napi::Number>(), info[1].As<Napi::Number>(), info[1].As<Napi::Number>());
    return;
  }
  adaptee = Robot::Bounds(info[0].As<Napi::Number>());
}

Napi::Value BoundsAdapter::normalize(const Napi::CallbackInfo& info) {
  auto o = Napi::Object::New(info.Env());
  auto b = NewAdaptee(info);
  o["x"] = b.X; o["y"] = b.Y; o["w"] = b.W; o["h"] = b.H;
  return o;
}

Napi::Value BoundsAdapter::x(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.X);
}
void BoundsAdapter::x(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.X = value.As<Napi::Number>();
}

Napi::Value BoundsAdapter::y(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Y);
}
void BoundsAdapter::y(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Y = value.As<Napi::Number>();
}

Napi::Value BoundsAdapter::w(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.W);
}
void BoundsAdapter::w(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.W = value.As<Napi::Number>();
}

Napi::Value BoundsAdapter::h(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.H);
}
void BoundsAdapter::h(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.H = value.As<Napi::Number>();
}

Napi::Value BoundsAdapter::isZero(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsZero());
}

Napi::Value BoundsAdapter::isEmpty(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsEmpty());
}

Napi::Value BoundsAdapter::isValid(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsValid());
}

Napi::Value BoundsAdapter::getLeft(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetLeft());
}

Napi::Value BoundsAdapter::getRight(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetRight());
}

Napi::Value BoundsAdapter::getTop(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetTop());
}

Napi::Value BoundsAdapter::getBottom(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetBottom());
}

void BoundsAdapter::setLeft(const Napi::CallbackInfo& info) {
  adaptee.SetLeft(info[0].As<Napi::Number>());
}

void BoundsAdapter::setRight(const Napi::CallbackInfo& info) {
  adaptee.SetRight(info[0].As<Napi::Number>());
}

void BoundsAdapter::setTop(const Napi::CallbackInfo& info) {
  adaptee.SetTop(info[0].As<Napi::Number>());
}

void BoundsAdapter::setBottom(const Napi::CallbackInfo& info) {
  adaptee.SetBottom(info[0].As<Napi::Number>());
}

Napi::Value BoundsAdapter::getLTRB(const Napi::CallbackInfo& info) {
  int32_t l, t, r, b;
  adaptee.GetLTRB(l, t, r, b);
  auto o = Napi::Object::New(env);
  o["l"] = l; o["t"] = t; o["r"] = r; o["b"] = b;
  return o;
}

void BoundsAdapter::setLTRB(const Napi::CallbackInfo& info) {
  adaptee.SetLTRB(info[0].As<Napi::Number>(), info[1].As<Napi::Number>(), info[2].As<Napi::Number>(), info[3].As<Napi::Number>());
}

void BoundsAdapter::normalizeSelf(const Napi::CallbackInfo& info) {
  adaptee.Normalize();
}

Napi::Value BoundsAdapter::containsP(const Napi::CallbackInfo& info) {
  auto inc = info[info.Length() - 1];
  return Napi::Boolean::New(env, adaptee.Contains(
    PointAdapter::NewAdaptee(info),
    inc.IsBoolean() ? inc.As<Napi::Boolean>() : true
  ));
}

Napi::Value BoundsAdapter::containsB(const Napi::CallbackInfo& info) {
  auto inc = info[info.Length() - 1];
  return Napi::Boolean::New(env, adaptee.Contains(
    NewAdaptee(info),
    inc.IsBoolean() ? inc.As<Napi::Boolean>() : true
  ));
}

Napi::Value BoundsAdapter::intersects(const Napi::CallbackInfo& info) {
  auto inc = info[info.Length() - 1];
  return Napi::Boolean::New(env, adaptee.Intersects(
    NewAdaptee(info),
    inc.IsBoolean() ? inc.As<Napi::Boolean>() : true
  ));
}

Napi::Value BoundsAdapter::getPoint(const Napi::CallbackInfo& info) {
  return PointAdapter::New(env, adaptee.GetPoint());
}

void BoundsAdapter::setPoint(const Napi::CallbackInfo& info) {
  adaptee.SetPoint(PointAdapter::NewAdaptee(info));
}

Napi::Value BoundsAdapter::getSize(const Napi::CallbackInfo& info) {
  return SizeAdapter::New(env, adaptee.GetSize());
}

void BoundsAdapter::setSize(const Napi::CallbackInfo& info) {
  adaptee.SetSize(SizeAdapter::NewAdaptee(info));
}

Napi::Value BoundsAdapter::getCenter(const Napi::CallbackInfo& info) {
  return PointAdapter::New(env, adaptee.GetCenter());
}

Napi::Value BoundsAdapter::unite(const Napi::CallbackInfo& info) {
  return New(env, adaptee | NewAdaptee(info));
}

Napi::Value BoundsAdapter::intersect(const Napi::CallbackInfo& info) {
  return New(env, adaptee & NewAdaptee(info));
}

Napi::Value BoundsAdapter::toString(const Napi::CallbackInfo& info) {
  auto stringstream = std::stringstream();
  stringstream << "[" <<
      adaptee.X << ", " <<
      adaptee.Y << ", " <<
      adaptee.W << ", " <<
      adaptee.H << "]";
  return Napi::String::New(env, stringstream.str());
}
