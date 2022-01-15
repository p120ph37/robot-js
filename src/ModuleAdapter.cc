#include "ModuleAdapter.h"
#include "ProcessAdapter.h"

Napi::Function ModuleAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Module", {
    InstanceAccessor("valid", &ModuleAdapter::valid, nullptr),
    InstanceAccessor("name", &ModuleAdapter::name, nullptr),
    InstanceAccessor("path", &ModuleAdapter::path, nullptr),
    InstanceAccessor("base", &ModuleAdapter::base, nullptr),
    InstanceAccessor("size", &ModuleAdapter::size, nullptr),
    InstanceAccessor("process", &ModuleAdapter::process, nullptr),
    InstanceMethod("contains", &ModuleAdapter::contains),
    InstanceMethod("lt", &ModuleAdapter::lt),
    InstanceMethod("gt", &ModuleAdapter::gt),
    InstanceMethod("le", &ModuleAdapter::le),
    InstanceMethod("ge", &ModuleAdapter::ge),
    InstanceMethod("eq", &ModuleAdapter::eq),
    InstanceMethod("ne", &ModuleAdapter::ne),
  });
}

ModuleAdapter::ModuleAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  throw Napi::TypeError::New(env, "Invalid arguments");
}

Napi::Value ModuleAdapter::valid(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsValid());
}

Napi::Value ModuleAdapter::name(const Napi::CallbackInfo& info) {
  return Napi::String::New(env, adaptee.GetName());
}

Napi::Value ModuleAdapter::path(const Napi::CallbackInfo& info) {
  return Napi::String::New(env, adaptee.GetPath());
}

Napi::Value ModuleAdapter::base(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetBase());
}

Napi::Value ModuleAdapter::size(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetSize());
}

Napi::Value ModuleAdapter::process(const Napi::CallbackInfo& info) {
  return ProcessAdapter::New(env, adaptee.GetProcess());
}

Napi::Value ModuleAdapter::contains(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Contains(info[0].As<Napi::Number>().Int64Value()));
}
