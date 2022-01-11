#include "HashAdapter.h"

Napi::Function HashAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Hash", {
    InstanceAccessor("result", &HashAdapter::result, &HashAdapter::result),
    InstanceMethod("append", &HashAdapter::append),
    InstanceMethod("eq", &HashAdapter::eq),
    InstanceMethod("ne", &HashAdapter::ne),
  });
}

HashAdapter::HashAdapter(const Napi::CallbackInfo& info) : ClassAdapterEq(info) {
  if(WrapAdaptee(info, adaptee)) return;
  if(IsInstance(info[0])) {
    adaptee = Unwrap(info[0])->adaptee;
    return;
  }
  if(info[0].IsUndefined()) {
    adaptee = Robot::Hash();
    return;
  }
  if(info[0].IsBuffer()) {
    auto buffer = info[0].As<Napi::Buffer<uint8_t>>();
    adaptee = Robot::Hash(buffer.Data(), buffer.Length());
  }
  if(info[0].IsNumber()) {
    adaptee = Robot::Hash();
    adaptee.Result = info[0].As<Napi::Number>();
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value HashAdapter::result(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Result);
}
void HashAdapter::result(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Result = value.ToNumber().Int32Value();
}

void HashAdapter::append(const Napi::CallbackInfo& info) {
  if(info[0].IsBuffer()) {
    auto buffer = info[0].As<Napi::Buffer<uint8_t>>();
    adaptee.Append(buffer.Data(), buffer.Length());
  }
  throw Napi::TypeError::New(env, "Invalid arguments");
}
