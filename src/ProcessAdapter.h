#include "ClassAdapter.h"
#include "Robot.h"

class ProcessAdapter :
  public ClassAdapter<ProcessAdapter, Robot::Process> {

  public:
    static Napi::Function Init(Napi::Env env);
    
    ProcessAdapter(const Napi::CallbackInfo& info);

    Napi::Value open(const Napi::CallbackInfo& info);
    void close(const Napi::CallbackInfo& info);
    Napi::Value isValid(const Napi::CallbackInfo& info);
    Napi::Value is64Bit(const Napi::CallbackInfo& info);
    Napi::Value isDebugged(const Napi::CallbackInfo& info);
    Napi::Value getPID(const Napi::CallbackInfo& info);
    Napi::Value getName(const Napi::CallbackInfo& info);
    Napi::Value getPath(const Napi::CallbackInfo& info);
    void exit(const Napi::CallbackInfo& info);
    void kill(const Napi::CallbackInfo& info);
    Napi::Value hasExited(const Napi::CallbackInfo& info);
    Napi::Value getModules(const Napi::CallbackInfo& info);
    Napi::Value getWindows(const Napi::CallbackInfo& info);
    static Napi::Value getList(const Napi::CallbackInfo& info);
    static Napi::Value getCurrent(const Napi::CallbackInfo& info);
    static Napi::Value isSys64Bit(const Napi::CallbackInfo& info);
    static Napi::Value getSegments(const Napi::CallbackInfo& info);

    Napi::Value eq(const Napi::CallbackInfo& info);
    Napi::Value ne(const Napi::CallbackInfo& info);

};
