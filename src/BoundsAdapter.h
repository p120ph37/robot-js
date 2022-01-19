#include "ClassAdapter.h"
#include "Robot.h"

class BoundsAdapter :
  public ClassAdapter<BoundsAdapter, Robot::Bounds>,
  public ClassAdapter<BoundsAdapter, Robot::Bounds>::Eq {

  public:
    static Napi::Function Init(Napi::Env env);
    
    BoundsAdapter(const Napi::CallbackInfo& info);

    static Napi::Value normalize(const Napi::CallbackInfo& info);

    Napi::Value x(const Napi::CallbackInfo& info);
    void x(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value y(const Napi::CallbackInfo& info);
    void y(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value w(const Napi::CallbackInfo& info);
    void w(const Napi::CallbackInfo& info, const Napi::Value& value);
    Napi::Value h(const Napi::CallbackInfo& info);
    void h(const Napi::CallbackInfo& info, const Napi::Value& value);

    Napi::Value isZero(const Napi::CallbackInfo& info);
    Napi::Value isEmpty(const Napi::CallbackInfo& info);
    Napi::Value isValid(const Napi::CallbackInfo& info);
    Napi::Value getLeft(const Napi::CallbackInfo& info);
    Napi::Value getRight(const Napi::CallbackInfo& info);
    Napi::Value getTop(const Napi::CallbackInfo& info);
    Napi::Value getBottom(const Napi::CallbackInfo& info);
    void setLeft(const Napi::CallbackInfo& info);
    void setRight(const Napi::CallbackInfo& info);
    void setTop(const Napi::CallbackInfo& info);
    void setBottom(const Napi::CallbackInfo& info);
    Napi::Value getLTRB(const Napi::CallbackInfo& info);
    void setLTRB(const Napi::CallbackInfo& info);
    void normalizeSelf(const Napi::CallbackInfo& info);
    Napi::Value containsP(const Napi::CallbackInfo& info);
    Napi::Value containsB(const Napi::CallbackInfo& info);
    Napi::Value intersects(const Napi::CallbackInfo& info);
    Napi::Value getPoint(const Napi::CallbackInfo& info);
    void setPoint(const Napi::CallbackInfo& info);
    Napi::Value getSize(const Napi::CallbackInfo& info);
    void setSize(const Napi::CallbackInfo& info);
    Napi::Value getCenter(const Napi::CallbackInfo& info);
    void setCenter(const Napi::CallbackInfo& info);
    Napi::Value unite(const Napi::CallbackInfo& info);
    Napi::Value intersect(const Napi::CallbackInfo& info);
    
    Napi::Value toString(const Napi::CallbackInfo& info);

};
