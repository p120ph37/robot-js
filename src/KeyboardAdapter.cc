#include "KeyboardAdapter.h"
#include "RangeAdapter.h"

Napi::Function KeyboardAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Keyboard", {
    InstanceAccessor("autoDelay", &KeyboardAdapter::autoDelay, &KeyboardAdapter::autoDelay),
    InstanceMethod("click", &KeyboardAdapter::click),
    InstanceMethod("press", &KeyboardAdapter::press),
    InstanceMethod("release", &KeyboardAdapter::release),
    StaticMethod("compile", &KeyboardAdapter::compile),
    StaticMethod("getState", &KeyboardAdapter::getState),
  });
}

KeyboardAdapter::KeyboardAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value KeyboardAdapter::autoDelay(const Napi::CallbackInfo& info) {
  return RangeAdapter::New(env, adaptee.AutoDelay);
}
void KeyboardAdapter::autoDelay(const Napi::CallbackInfo& info, const Napi::Value& value) {
  if(RangeAdapter::IsInstance(info[0])) {
    adaptee.AutoDelay = RangeAdapter::Unwrap(info[0])->adaptee;
    return;
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}

void KeyboardAdapter::click(const Napi::CallbackInfo& info) {
  if(info[0].IsString()) {
    adaptee.Click(info[0].As<Napi::String>().Utf8Value().c_str());
    return;
  }
  adaptee.Click((Robot::Key)info[0].As<Napi::Number>().Int32Value());
}

void KeyboardAdapter::press(const Napi::CallbackInfo& info) {
  adaptee.Press((Robot::Key)info[0].As<Napi::Number>().Int32Value());
}

void KeyboardAdapter::release(const Napi::CallbackInfo& info) {
  adaptee.Release((Robot::Key)info[0].As<Napi::Number>().Int32Value());
}

Napi::Value KeyboardAdapter::compile(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  Robot::KeyList keys;
  if(Robot::Keyboard::Compile(info[0].As<Napi::String>().Utf8Value().c_str(), keys)) {
    auto arr = Napi::Array::New(env, keys.size());
    for(size_t i = 0; i < keys.size(); i++) {
      auto o = Napi::Object::New(env);
      o["down"] = keys[i].first;
      o["key"] = (uint32_t)keys[i].second;
      arr[i] = o;
    }
    return arr;
  }
  return Napi::Array::New(env);
}

Napi::Value KeyboardAdapter::getState(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  if(info[0].IsUndefined()) {
    auto o = Napi::Object::New(env);
    Robot::KeyState state;
    if(Robot::Keyboard::GetState(state)) {
      for(auto el : state) {
        o.Set(el.first, Napi::Boolean::New(env, el.second));
      }
    }
    return o;
  }
  return Napi::Boolean::New(env, Robot::Keyboard::GetState((Robot::Key)info[0].As<Napi::Number>().Int32Value()));
}
