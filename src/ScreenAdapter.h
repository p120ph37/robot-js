#include "ClassAdapter.h"
#include "Robot.h"

class ScreenAdapter :
  public ClassAdapter<ScreenAdapter, Robot::Screen> {

  public:
    static Napi::Function Init(Napi::Env env);
    
    ScreenAdapter(const Napi::CallbackInfo& info);

    Napi::Value getBounds(const Napi::CallbackInfo& info);
    Napi::Value getUsable(const Napi::CallbackInfo& info);
    Napi::Value isPortrait(const Napi::CallbackInfo& info);
    Napi::Value isLandscape(const Napi::CallbackInfo& info);
    static Napi::Value synchronize(const Napi::CallbackInfo& info);
    static Napi::Value getMain(const Napi::CallbackInfo& info);
    static Napi::Value getList(const Napi::CallbackInfo& info);
    static Napi::Value getScreen(const Napi::CallbackInfo& info);
    static Napi::Value grabScreen(const Napi::CallbackInfo& info);
    static Napi::Value getTotalBounds(const Napi::CallbackInfo& info);
    static Napi::Value getTotalUsable(const Napi::CallbackInfo& info);
    static Napi::Value isCompositing(const Napi::CallbackInfo& info);
    static void setCompositing(const Napi::CallbackInfo& info);

};
