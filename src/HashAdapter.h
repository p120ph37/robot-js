#include "ClassAdapter.h"
#include "Robot.h"

class HashAdapter :
  public ClassAdapter<HashAdapter, Robot::Hash> {

  public:
    static Napi::Function Init(Napi::Env env);
    
    HashAdapter(const Napi::CallbackInfo& info);

    Napi::Value result(const Napi::CallbackInfo& info);
    void result(const Napi::CallbackInfo& info, const Napi::Value& value);

    void append(const Napi::CallbackInfo& info);

    Napi::Value eq(const Napi::CallbackInfo& info);
    Napi::Value ne(const Napi::CallbackInfo& info);

    Napi::Value toString(const Napi::CallbackInfo& info);

};
