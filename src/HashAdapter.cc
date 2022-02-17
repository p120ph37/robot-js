#include "HashAdapter.h"
#include <sstream>

Napi::Function HashAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Hash", {
    InstanceAccessor("result", &HashAdapter::result, &HashAdapter::result),
    InstanceMethod("append", &HashAdapter::append),
    InstanceMethod("eq", &HashAdapter::eq),
    InstanceMethod("ne", &HashAdapter::ne),
    InstanceMethod("toString", &HashAdapter::toString),
  });
}

namespace {
  Napi::Value ToPrimitive(Napi::Value v) {
    if(!v.IsObject()) return v;
    auto o = v.As<Napi::Object>();
    auto valueOf = "valueOf";
    if(o.Has(valueOf) && o.Get(valueOf).IsFunction()) {
      Napi::Value p = o.Get(valueOf).As<Napi::Function>().Call(o, {});
      if(!p.StrictEquals(o)) return p;
    }
    auto toPrimitive = Napi::Symbol::WellKnown(o.Env(), "toPrimitive");
    if(o.Has(toPrimitive) && o.Get(toPrimitive).IsFunction()) {
      return o.Get(toPrimitive).As<Napi::Function>().Call(o, {});
    }
    return o;
  }
}

HashAdapter::HashAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  if(info[0].IsNumber()) {
    adaptee = Robot::Hash();
    adaptee.Result = info[0].As<Napi::Number>();
    return;
  }
  adaptee = Robot::Hash();
  append(info);
}

Napi::Value HashAdapter::result(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Result);
}
void HashAdapter::result(const Napi::CallbackInfo& info, const Napi::Value& value) {
  adaptee.Result = value.As<Napi::Number>().Int32Value();
}

void HashAdapter::append(const Napi::CallbackInfo& info) {
  if(info[0].IsUndefined()) throw Napi::TypeError::New(env, "Invalid arguments");
  auto p = ToPrimitive(info[0]);
  if(p.IsBuffer()) {
    auto buffer = p.As<Napi::Buffer<uint8_t>>();
    adaptee.Append(buffer.Data(), buffer.Length());
    return;
  }
  if(p.IsArray()) {
    auto arr = p.As<Napi::Array>();
    std::vector<Robot::uint8> data(arr.Length());
    for(size_t i = 0; i < arr.Length(); i++) {
      data[i] = arr.Get(i).As<Napi::Number>().Int32Value();
    }
    adaptee.Append(data.data(), data.size());
    return;
  }
  if(p.IsArrayBuffer()) {
    auto ab = p.As<Napi::ArrayBuffer>();
    adaptee.Append((Robot::uint8*)ab.Data(), ab.ByteLength());
    return;
  }
  auto str = p.ToString().Utf8Value();
  adaptee.Append((Robot::uint8*)str.c_str(), str.length());
}

Napi::Value HashAdapter::eq(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Result == (info[0].IsNumber() ?
    info[0].As<Napi::Number>().Int32Value() :
    Unwrap(info[0])->adaptee.Result
  ));
}
Napi::Value HashAdapter::ne(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Result != (info[0].IsNumber() ?
    info[0].As<Napi::Number>().Int32Value() :
    Unwrap(info[0])->adaptee.Result
  ));
}

Napi::Value HashAdapter::toString(const Napi::CallbackInfo& info) {
  auto stringstream = std::stringstream();
  stringstream << "0x" << std::uppercase << std::hex << adaptee.Result;
  return Napi::String::New(env, stringstream.str());
}
