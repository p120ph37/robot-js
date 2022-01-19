#include "ClassAdapter.h"
#include "Robot.h"

class ColorAdapter :
  public ClassAdapter<ColorAdapter, Robot::Color>,
  public ClassAdapter<ColorAdapter, Robot::Color>::Eq {

  public:
    static Napi::Function Init(Napi::Env env);
    
    ColorAdapter(const Napi::CallbackInfo& info);

    static Napi::Value normalize(const Napi::CallbackInfo& info);

    Napi::Value a(const Napi::CallbackInfo& info);
    void a(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value r(const Napi::CallbackInfo& info);
    void r(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value g(const Napi::CallbackInfo& info);
    void g(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value b(const Napi::CallbackInfo& info);
    void b(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value getARGB(const Napi::CallbackInfo& info);
    void setARGB(const Napi::CallbackInfo& info);

    Napi::Value toString(const Napi::CallbackInfo& info);

};
