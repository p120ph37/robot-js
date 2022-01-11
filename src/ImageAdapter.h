#include "ClassAdapter.h"
#include "Robot.h"

class ImageAdapter : public ClassAdapterEq<ImageAdapter, Robot::Image> {
  public:
    static Napi::Function Init(Napi::Env env);

    ImageAdapter(const Napi::CallbackInfo& info);

    Napi::Value isValid(const Napi::CallbackInfo& info);
    Napi::Value create(const Napi::CallbackInfo& info);
    void destroy(const Napi::CallbackInfo& info);
    Napi::Value getWidth(const Napi::CallbackInfo& info);
    Napi::Value getHeight(const Napi::CallbackInfo& info);
    Napi::Value getLength(const Napi::CallbackInfo& info);
    Napi::Value getData(const Napi::CallbackInfo& info);
    Napi::Value getLimit(const Napi::CallbackInfo& info);
    Napi::Value getPixel(const Napi::CallbackInfo& info);
    void setPixel(const Napi::CallbackInfo& info);
    Napi::Value fill(const Napi::CallbackInfo& info);
    Napi::Value swap(const Napi::CallbackInfo& info);
    Napi::Value flip(const Napi::CallbackInfo& info);
};
