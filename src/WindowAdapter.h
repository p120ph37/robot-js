#include "ClassAdapter.h"
#include "Robot.h"

class WindowAdapter :
  public ClassAdapter<WindowAdapter, Robot::Window>  {

  public:
    static Napi::Function Init(Napi::Env env);
    
    WindowAdapter(const Napi::CallbackInfo& info);

    Napi::Value isValid(const Napi::CallbackInfo& info);
    void close(const Napi::CallbackInfo& info);
    Napi::Value isTopMost(const Napi::CallbackInfo& info);
    Napi::Value isBorderless(const Napi::CallbackInfo& info);
    Napi::Value isMinimized(const Napi::CallbackInfo& info);
    Napi::Value isMaximized(const Napi::CallbackInfo& info);
    void setTopMost(const Napi::CallbackInfo& info);
    void setBorderless(const Napi::CallbackInfo& info);
    void setMinimized(const Napi::CallbackInfo& info);
    void setMaximized(const Napi::CallbackInfo& info);
    Napi::Value getProcess(const Napi::CallbackInfo& info);
    Napi::Value getPID(const Napi::CallbackInfo& info);
    Napi::Value getHandle(const Napi::CallbackInfo& info);
    Napi::Value setHandle(const Napi::CallbackInfo& info);
    Napi::Value getTitle(const Napi::CallbackInfo& info);
    void setTitle(const Napi::CallbackInfo& info);
    Napi::Value getBounds(const Napi::CallbackInfo& info);
    void setBounds(const Napi::CallbackInfo& info);
    Napi::Value getClient(const Napi::CallbackInfo& info);
    void setClient(const Napi::CallbackInfo& info);
    Napi::Value mapToClient(const Napi::CallbackInfo& info);
    Napi::Value mapToScreen(const Napi::CallbackInfo& info);
    static Napi::Value getList(const Napi::CallbackInfo& info);
    static Napi::Value getActive(const Napi::CallbackInfo& info);
    static void setActive(const Napi::CallbackInfo& info);
    static Napi::Value isAxEnabled(const Napi::CallbackInfo& info);

    Napi::Value eq(const Napi::CallbackInfo& info);
    Napi::Value ne(const Napi::CallbackInfo& info);

};
