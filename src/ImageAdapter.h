#include "ClassAdapter.h"
#include "Robot.h"

class ImageAdapter :
  public ClassAdapter<ImageAdapter, std::shared_ptr<Robot::Image>> {

  private:
    Napi::Reference<Napi::Value> bufferRef;
    void clear();
    void realloc();

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
    Napi::Value eq(const Napi::CallbackInfo& info);
    Napi::Value ne(const Napi::CallbackInfo& info);

    Napi::Value toString(const Napi::CallbackInfo& info);

};
