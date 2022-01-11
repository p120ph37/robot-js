#include <napi.h>

class RobotAdapter : public Napi::Addon<RobotAdapter> {
  public:
    RobotAdapter(Napi::Env env, Napi::Object exports);
};
