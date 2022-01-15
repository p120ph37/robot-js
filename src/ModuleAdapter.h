#include "ClassAdapter.h"
#include "Robot.h"

class ModuleAdapter :
  public ClassAdapter<ModuleAdapter, Robot::Module>,
  public ClassAdapter<ModuleAdapter, Robot::Module>::Eq,
  public ClassAdapter<ModuleAdapter, Robot::Module>::Cmp {

  public:
    static Napi::Function Init(Napi::Env env);
    
    ModuleAdapter(const Napi::CallbackInfo& info);

    Napi::Value valid(const Napi::CallbackInfo& info);
    Napi::Value name(const Napi::CallbackInfo& info);
    Napi::Value path(const Napi::CallbackInfo& info);
    Napi::Value base(const Napi::CallbackInfo& info);
    Napi::Value size(const Napi::CallbackInfo& info);
    Napi::Value process(const Napi::CallbackInfo& info);

    Napi::Value contains(const Napi::CallbackInfo& info);

 };
