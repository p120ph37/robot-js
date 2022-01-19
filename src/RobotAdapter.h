#include <napi.h>

#define ADDON_VERSION 0x020000
#define ADDON_VERSION_STR "2.0.0-napi"

class RobotAdapter :
  public Napi::Addon<RobotAdapter> {

  public:
    RobotAdapter(Napi::Env env, Napi::Object exports);

};
