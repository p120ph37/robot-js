#include "ClassAdapter.h"
#include "Robot.h"

class SizeAdapter :
  public ClassAdapter<SizeAdapter, Robot::Size>,
  public ClassAdapter<SizeAdapter, Robot::Size>::Eq {

  public:
    static Napi::Function Init(Napi::Env env);
    
    SizeAdapter(const Napi::CallbackInfo& info);

    static Napi::Value normalize(const Napi::CallbackInfo& info);

    Napi::Value w(const Napi::CallbackInfo& info);
    void w(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value h(const Napi::CallbackInfo& info);
    void h(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value isZero(const Napi::CallbackInfo& info);
    Napi::Value isEmpty(const Napi::CallbackInfo& info);
    Napi::Value toPoint(const Napi::CallbackInfo& info);
    Napi::Value add(const Napi::CallbackInfo& info);
    Napi::Value sub(const Napi::CallbackInfo& info);

    Napi::Value toString(const Napi::CallbackInfo& info);

};
