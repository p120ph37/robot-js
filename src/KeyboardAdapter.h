#include "ClassAdapter.h"
#include "Robot.h"

class KeyboardAdapter : public ClassAdapter<KeyboardAdapter, Robot::Keyboard> {
  public:
    static Napi::Function Init(Napi::Env env);
    
    KeyboardAdapter(const Napi::CallbackInfo& info);

    Napi::Value autoDelay(const Napi::CallbackInfo& info);
    void autoDelay(const Napi::CallbackInfo& info, const Napi::Value& value);

    void click(const Napi::CallbackInfo& info);
    void press(const Napi::CallbackInfo& info);
    void release(const Napi::CallbackInfo& info);
    Napi::Value compile(const Napi::CallbackInfo& info);
    Napi::Value getState(const Napi::CallbackInfo& info);
};
