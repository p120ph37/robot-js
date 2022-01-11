#include "ClassAdapter.h"
#include "Robot.h"

class TimerAdapter : public ClassAdapterCmp<TimerAdapter, Robot::Timer> {
  public:
    static Napi::Function Init(Napi::Env env);
    
    TimerAdapter(const Napi::CallbackInfo& info);

    void start(const Napi::CallbackInfo& info);
    Napi::Value reset(const Napi::CallbackInfo& info);
    Napi::Value restart(const Napi::CallbackInfo& info);
    Napi::Value getElapsed(const Napi::CallbackInfo& info);
    Napi::Value hasStarted(const Napi::CallbackInfo& info);
    Napi::Value hasExpired(const Napi::CallbackInfo& info);
    static void sleep(const Napi::CallbackInfo& info);
    static Napi::Value getCpuTime(const Napi::CallbackInfo& info);
};
