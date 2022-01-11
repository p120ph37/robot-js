#include "ClassAdapter.h"
#include "Robot.h"

class SizeAdapter : public ClassAdapterEq<SizeAdapter, Robot::Size> {
  public:
    static Napi::Function Init(Napi::Env env);
    
    SizeAdapter(const Napi::CallbackInfo& info);

    Napi::Value w(const Napi::CallbackInfo& info);
    void w(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value h(const Napi::CallbackInfo& info);
    void h(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value isZero(const Napi::CallbackInfo& info);
    Napi::Value toPoint(const Napi::CallbackInfo& info);
    Napi::Value add(const Napi::CallbackInfo& info);
    Napi::Value sub(const Napi::CallbackInfo& info);
 };
