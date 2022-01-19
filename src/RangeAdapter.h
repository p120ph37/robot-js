#include "ClassAdapter.h"
#include "Robot.h"

class RangeAdapter :
  public ClassAdapter<RangeAdapter, Robot::Range>,
  public ClassAdapter<RangeAdapter, Robot::Range>::Eq {

  public:
    static Napi::Function Init(Napi::Env env);
    
    RangeAdapter(const Napi::CallbackInfo& info);

    static Napi::Value normalize(const Napi::CallbackInfo& info);

    Napi::Value min(const Napi::CallbackInfo& info);
    void min(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value max(const Napi::CallbackInfo& info);
    void max(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value getRange(const Napi::CallbackInfo& info);
    void setRange(const Napi::CallbackInfo& info);
    Napi::Value getRandom(const Napi::CallbackInfo& info);
    Napi::Value contains(const Napi::CallbackInfo& info);

    Napi::Value toString(const Napi::CallbackInfo& info);

};
