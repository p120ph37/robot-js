#include "ClassAdapter.h"
#include "Robot.h"

class PointAdapter : public ClassAdapterEq<PointAdapter, Robot::Point> {
  public:
    static Napi::Function Init(Napi::Env env);
    
    PointAdapter(const Napi::CallbackInfo& info);

    Napi::Value x(const Napi::CallbackInfo& info);
    void x(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value y(const Napi::CallbackInfo& info);
    void y(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value isZero(const Napi::CallbackInfo& info);
    Napi::Value toSize(const Napi::CallbackInfo& info);
    Napi::Value add(const Napi::CallbackInfo& info);
    Napi::Value sub(const Napi::CallbackInfo& info);
    Napi::Value neg(const Napi::CallbackInfo& info);
 };
