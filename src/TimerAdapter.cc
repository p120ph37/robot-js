#include "TimerAdapter.h"
#include "RangeAdapter.h"

Napi::Function TimerAdapter::Init(Napi::Env env) {
  return DefineClass(env, "Timer", {
    InstanceMethod("start", &TimerAdapter::start),
    InstanceMethod("reset", &TimerAdapter::reset),
    InstanceMethod("restart", &TimerAdapter::restart),
    InstanceMethod("getElapsed", &TimerAdapter::getElapsed),
    InstanceMethod("hasStarted", &TimerAdapter::hasStarted),
    InstanceMethod("hasExpired", &TimerAdapter::hasExpired),
    InstanceMethod("lt", &TimerAdapter::lt),
    InstanceMethod("gt", &TimerAdapter::gt),
    InstanceMethod("le", &TimerAdapter::le),
    InstanceMethod("ge", &TimerAdapter::ge),
    InstanceMethod("eq", &TimerAdapter::eq),
    InstanceMethod("ne", &TimerAdapter::ne),
    StaticMethod("sleep", &TimerAdapter::sleep),
    StaticMethod("getCpuTime", &TimerAdapter::getCpuTime),
  });
}

TimerAdapter::TimerAdapter(const Napi::CallbackInfo& info) : ClassAdapter(info) {
  if(WrapAdaptee(info) || ConstructDefault(info) || CopyThat(info)) return;
  throw Napi::TypeError::New(env, "Invalid arguments");
}

void TimerAdapter::start(const Napi::CallbackInfo& info) {
  adaptee.Start();
}

Napi::Value TimerAdapter::reset(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Reset());
}

Napi::Value TimerAdapter::restart(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.Restart());
}

Napi::Value TimerAdapter::getElapsed(const Napi::CallbackInfo& info) {
  return Napi::Number::New(env, adaptee.GetElapsed());
}

Napi::Value TimerAdapter::hasStarted(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.HasStarted());
}

Napi::Value TimerAdapter::hasExpired(const Napi::CallbackInfo& info) {
  return Napi::Boolean::New(env, adaptee.HasExpired(info[0].As<Napi::Number>().Int64Value()));
}

void TimerAdapter::sleep(const Napi::CallbackInfo& info) {
  Robot::Timer::Sleep(RangeAdapter::NewAdaptee(info));
}

Napi::Value TimerAdapter::getCpuTime(const Napi::CallbackInfo& info) {
  return Napi::Number::New(info.Env(), Robot::Timer::GetCpuTime());
}
