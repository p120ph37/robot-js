#include "ProcessAdapter.h"
#include "ModuleAdapter.h"
#include "WindowAdapter.h"

Napi::Function ProcessAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Process", {
    InstanceMethod("open", &ProcessAdapter::open),
    InstanceMethod("close", &ProcessAdapter::close),
    InstanceMethod("isValid", &ProcessAdapter::isValid),
    InstanceMethod("is64Bit", &ProcessAdapter::is64Bit),
    InstanceMethod("isDebugged", &ProcessAdapter::isDebugged),
    InstanceMethod("getPID", &ProcessAdapter::getPID),
    InstanceMethod("getName", &ProcessAdapter::getName),
    InstanceMethod("getPath", &ProcessAdapter::getPath),
    InstanceMethod("exit", &ProcessAdapter::exit),
    InstanceMethod("kill", &ProcessAdapter::kill),
    InstanceMethod("hasExited", &ProcessAdapter::hasExited),
    InstanceMethod("getModules", &ProcessAdapter::getModules),
    InstanceMethod("getWindows", &ProcessAdapter::getWindows),
    StaticMethod("getList", &ProcessAdapter::getList),
    StaticMethod("getCurrent", &ProcessAdapter::getCurrent),
    StaticMethod("isSys64Bit", &ProcessAdapter::isSys64Bit),
    StaticMethod("_getSegments", &ProcessAdapter::getSegments),
    InstanceMethod("eq", &ProcessAdapter::eq),
    InstanceMethod("ne", &ProcessAdapter::ne),
  });
}

ProcessAdapter::ProcessAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  adaptee = Robot::Process(info[0].As<Napi::Number>());
}

Napi::Value ProcessAdapter::open(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Open(info[0].As<Napi::Number>()));
}

void ProcessAdapter::close(const Napi::CallbackInfo& info) {
  adaptee.Close();
}

Napi::Value ProcessAdapter::isValid(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsValid());
}

Napi::Value ProcessAdapter::is64Bit(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.Is64Bit());
}

Napi::Value ProcessAdapter::isDebugged(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.IsDebugged());
}

Napi::Value ProcessAdapter::getPID(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetPID());
}

Napi::Value ProcessAdapter::getName(const Napi::CallbackInfo& info) {
  return Napi::String::New(env, adaptee.GetName());
}

Napi::Value ProcessAdapter::getPath(const Napi::CallbackInfo& info) {
  return Napi::String::New(env, adaptee.GetPath());
}

void ProcessAdapter::exit(const Napi::CallbackInfo& info) {
  adaptee.Exit();
}

void ProcessAdapter::kill(const Napi::CallbackInfo& info) {
  adaptee.Kill();
}

Napi::Value ProcessAdapter::hasExited(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.HasExited());
}

Napi::Value ProcessAdapter::getModules(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  auto modules = info[0].IsUndefined() ? adaptee.GetModules() : adaptee.GetModules(info[0].As<Napi::String>().Utf8Value().c_str());
  auto arr = Napi::Array::New(env, modules.size());
  for(size_t i = 0; i < modules.size(); i++) {
    arr[i] = ModuleAdapter::New(env, modules[i]);
  }
  return arr;
}

Napi::Value ProcessAdapter::getWindows(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  auto windows = info[0].IsUndefined() ? adaptee.GetWindows() : adaptee.GetWindows(info[0].As<Napi::String>().Utf8Value().c_str());
  auto arr = Napi::Array::New(env, windows.size());
  for(size_t i = 0; i < windows.size(); i++) {
    arr[i] = WindowAdapter::New(env, windows[i]);
  }
  return arr;
}

Napi::Value ProcessAdapter::getList(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  auto processes = info[0].IsUndefined() ?
    Robot::Process::GetList() :
    Robot::Process::GetList(info[0].As<Napi::String>().Utf8Value().c_str());
  auto arr = Napi::Array::New(env, processes.size());
  for(size_t i = 0; i < processes.size(); i++) {
    arr[i] = New(env, processes[i]);
  }
  return arr;
}

Napi::Value ProcessAdapter::getCurrent(const Napi::CallbackInfo& info) {
  return New(info.Env(), Robot::Process::GetCurrent());
}

Napi::Value ProcessAdapter::isSys64Bit(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(info.Env(), Robot::Process::IsSys64Bit());
}

Napi::Value ProcessAdapter::getSegments(const Napi::CallbackInfo& info) {
  auto env = info.Env();
  if(!IsInstance(info[0])) throw Napi::TypeError::New(env, "Invalid arguments");
  auto proc = Unwrap(info[0])->adaptee;

  // Synthesize a module to get segments
  Robot::Module module(proc, "", "",
    (Robot::uintptr) info[1].As<Napi::Number>().DoubleValue(), 0);
  auto list = module.GetSegments();

  auto arr = Napi::Array::New(env, list.size());
  for(size_t i = 0; i < list.size(); ++i) {
    auto obj = Napi::Object::New(env);
    obj["valid"] = Napi::Boolean::New(env, list[i].Valid);
    obj["base"]  = Napi::Number::New(env, (double) list[i].Base);
    obj["size"]  = Napi::Number::New(env, (double) list[i].Size);
    obj["name"]  = Napi::String::New(env, list[i].Name);
    arr[i] = obj;
  }
  return arr;
}

Napi::Value ProcessAdapter::eq(const Napi::CallbackInfo& info) {
  if(IsInstance(info[0])) {
    return Napi::Boolean::New(env, adaptee == Unwrap(info[0])->adaptee);
  } else {
    return Napi::Boolean::New(env, adaptee.GetPID() == info[0].As<Napi::Number>().Int64Value());
  }
}

Napi::Value ProcessAdapter::ne(const Napi::CallbackInfo& info) {
  if(IsInstance(info[0])) {
    return Napi::Boolean::New(env, adaptee != Unwrap(info[0])->adaptee);
  } else {
    return Napi::Boolean::New(env, adaptee.GetPID() != info[0].As<Napi::Number>().Int64Value());
  }
}
