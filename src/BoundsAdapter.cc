#include "BoundsAdapter.h"
#include "PointAdapter.h"
#include "SizeAdapter.h"

Napi::Function BoundsAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Bounds", {
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
    InstanceMethod("normalize", &BoundsAdapter::normalize),
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
  });
}

BoundsAdapter::BoundsAdapter(const Napi::CallbackInfo& info) : ClassAdapterEq(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Bounds();
    return;
  }
  if(PointAdapter::IsInstance(info[0]) && SizeAdapter::IsInstance(info[1])) {
    adaptee = Robot::Bounds(PointAdapter::Unwrap(info[0])->adaptee, SizeAdapter::Unwrap(info[1])->adaptee);
    return;
  }
  if(info[0].IsObject()) {
    auto o = info[0].As<Napi::Object>();
    if(o.Has("x") && o.Has("y") && o.Has("w") && o.Has("h")) {
      adaptee = Robot::Bounds(o.Get("x").ToNumber(), o.Get("y").ToNumber(), o.Get("w").ToNumber(), o.Get("h").ToNumber());
      return;
    }
    if(o.Has("l") && o.Has("t") && o.Has("r") && o.Has("b")) {
      adaptee = Robot::Bounds();
      adaptee.SetLTRB(o.Get("l").ToNumber(), o.Get("t").ToNumber(), o.Get("r").ToNumber(), o.Get("b").ToNumber());
      return;
    }
  }
  if(!info[1].IsUndefined()) {
    adaptee = Robot::Bounds(info[0].ToNumber(), info[1].ToNumber(), info[2].ToNumber(), info[3].ToNumber());
    return;
  }
  adaptee = Robot::Bounds(info[0].ToNumber());
}

Napi::Value BoundsAdapter::x(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.X);
}
void BoundsAdapter::x(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.X = value.ToNumber();
}

Napi::Value BoundsAdapter::y(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Y);
}
void BoundsAdapter::y(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Y = value.ToNumber();
}

Napi::Value BoundsAdapter::w(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.W);
}
void BoundsAdapter::w(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.W = value.ToNumber();
}

Napi::Value BoundsAdapter::h(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.H);
}
void BoundsAdapter::h(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.H = value.ToNumber();
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
  adaptee.SetLeft(info[0].ToNumber());
}

void BoundsAdapter::setRight(const Napi::CallbackInfo& info) {
  adaptee.SetRight(info[0].ToNumber());
}

void BoundsAdapter::setTop(const Napi::CallbackInfo& info) {
  adaptee.SetTop(info[0].ToNumber());
}

void BoundsAdapter::setBottom(const Napi::CallbackInfo& info) {
  adaptee.SetBottom(info[0].ToNumber());
}

Napi::Value BoundsAdapter::getLTRB(const Napi::CallbackInfo& info) {
  int32_t l, t, r, b;
  adaptee.GetLTRB(l, t, r, b);
  auto o = Napi::Object::New(env);
  o["l"] = l; o["t"] = t; o["r"] = r; o["b"] = b;
  return o;
}

void BoundsAdapter::setLTRB(const Napi::CallbackInfo& info) {
  adaptee.SetLTRB(info[0].ToNumber(), info[1].ToNumber(), info[2].ToNumber(), info[3].ToNumber());
}

void BoundsAdapter::normalize(const Napi::CallbackInfo& info) {
  adaptee.Normalize();
}

Napi::Value BoundsAdapter::containsP(const Napi::CallbackInfo& info) {
  auto inc(info[info.Length() - 1]);
  return Napi::Boolean::New(env, adaptee.Contains(
    PointAdapter(info).adaptee,
    inc.IsBoolean() ? inc.As<Napi::Boolean>() : true
  ));
}

Napi::Value BoundsAdapter::containsB(const Napi::CallbackInfo& info) {
  auto inc(info[info.Length() - 1]);
  return Napi::Boolean::New(env, adaptee.Contains(
    BoundsAdapter(info).adaptee,
    inc.IsBoolean() ? inc.As<Napi::Boolean>() : true
  ));
}

Napi::Value BoundsAdapter::intersects(const Napi::CallbackInfo& info) {
  auto inc(info[info.Length() - 1]);
  return Napi::Boolean::New(env, adaptee.Intersects(
    BoundsAdapter(info).adaptee,
    inc.IsBoolean() ? inc.As<Napi::Boolean>() : true
  ));
}

Napi::Value BoundsAdapter::getPoint(const Napi::CallbackInfo& info) {
  return PointAdapter::New(env, adaptee.GetPoint());
}

void BoundsAdapter::setPoint(const Napi::CallbackInfo& info) {
  adaptee.SetPoint(PointAdapter(info).adaptee);
}

Napi::Value BoundsAdapter::getSize(const Napi::CallbackInfo& info) {
  return SizeAdapter::New(env, adaptee.GetSize());
}

void BoundsAdapter::setSize(const Napi::CallbackInfo& info) {
  adaptee.SetSize(SizeAdapter(info).adaptee);
}

Napi::Value BoundsAdapter::getCenter(const Napi::CallbackInfo& info) {
  return PointAdapter::New(env, adaptee.GetCenter());
}

Napi::Value BoundsAdapter::unite(const Napi::CallbackInfo& info) {
  return New(env, adaptee | BoundsAdapter(info).adaptee);
}

Napi::Value BoundsAdapter::intersect(const Napi::CallbackInfo& info) {
  return New(env, adaptee & BoundsAdapter(info).adaptee);
}
