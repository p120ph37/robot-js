#include "ClassAdapter.h"
#include "Robot.h"

class MouseAdapter :
  public ClassAdapter<MouseAdapter, Robot::Mouse> {

  public:
    static Napi::Function Init(Napi::Env env);
    
    MouseAdapter(const Napi::CallbackInfo& info);

    Napi::Value autoDelay(const Napi::CallbackInfo& info);
    void autoDelay(const Napi::CallbackInfo& info, const Napi::Value& value);

    void click(const Napi::CallbackInfo& info);
    void press(const Napi::CallbackInfo& info);
    void release(const Napi::CallbackInfo& info);
    void scrollH(const Napi::CallbackInfo& info);
    void scrollV(const Napi::CallbackInfo& info);
    static Napi::Value getPos(const Napi::CallbackInfo& info);
    static void setPos(const Napi::CallbackInfo& info);
    static Napi::Value getState(const Napi::CallbackInfo& info);

};
