#pragma once

#include "EnvLocal.h"
#include <napi.h>

template <typename T, typename A>
class ClassAdapter : public Napi::ObjectWrap<T> {
  private:
    static EnvLocal<Napi::Function> env_local_ctor;
    static EnvLocal<Napi::Symbol> env_local_adaptee_symbol;
    inline static void LazyInitEnv(napi_env env) {
      if(env_local_ctor.has(env)) return;
      env_local_ctor.set(T::Init(env));
      env_local_adaptee_symbol.set(Napi::Symbol::New(env, "adaptee"));
    }
  protected:
    Napi::Env env;
  public:
//    static Napi::Function Init(napi_env env);
    inline static Napi::Function GetConstructor(napi_env env) {
      LazyInitEnv(env);
      return env_local_ctor.get(env);
    }
    // Utility function for use in constructor to allow retrieval of raw adaptee if passed internally.
    inline static bool WrapAdaptee(const Napi::CallbackInfo& info, A& adaptee) {
      if(env_local_adaptee_symbol.get(info.Env()).StrictEquals(info[0]) && info[1].IsExternal()) {
        adaptee = *info[1].As<Napi::External<A>>().Data();
        return true;
      }
      return false;
    }
    // Will only work for classes which use `if(WrapAdaptee(info, adaptee)) return;` in their constructor:
    inline static Napi::Value New(napi_env env, A const &adaptee) {
      return GetConstructor(env).New({
        env_local_adaptee_symbol.get(env),
        Napi::External<A>::New(env, (A*)&adaptee)
      });
    }
    inline static bool IsInstance(Napi::Value value) {
      return value.IsObject() && value.As<Napi::Object>().InstanceOf(GetConstructor(value.Env()));
    }
    inline static T* Unwrap(Napi::Value obj) {
      return Napi::ObjectWrap<T>::Unwrap(obj.As<Napi::Object>());
    }
    A adaptee;
    inline ClassAdapter(const Napi::CallbackInfo& info) :
      Napi::ObjectWrap<T>(info),
      env(info.Env()) {}
};

template <typename T, typename A>
EnvLocal<Napi::Function> ClassAdapter<T, A>::env_local_ctor = {};
template <typename T, typename A>
EnvLocal<Napi::Symbol> ClassAdapter<T, A>::env_local_adaptee_symbol = {};

template <typename T, typename A>
class ClassAdapterEq : public ClassAdapter<T, A> {
  public:
    inline ClassAdapterEq(const Napi::CallbackInfo& info) : ClassAdapter<T, A>(info) {}
    virtual inline Napi::Value eq(const Napi::CallbackInfo& info) {
      return Napi::Boolean::New(this->env, this->adaptee == ClassAdapter<T, A>(info).adaptee);
    }
    virtual inline Napi::Value ne(const Napi::CallbackInfo& info) {
      return Napi::Boolean::New(this->env, this->adaptee != ClassAdapter<T, A>(info).adaptee);
    }
};

template <typename T, typename A>
class ClassAdapterCmp : public ClassAdapterEq<T, A> {
  public:
    inline ClassAdapterCmp(const Napi::CallbackInfo& info) : ClassAdapterEq<T, A>(info) {}
    virtual inline Napi::Value lt(const Napi::CallbackInfo& info) {
      return Napi::Boolean::New(this->env, this->adaptee < ClassAdapter<T, A>(info).adaptee);
    }
    virtual inline Napi::Value gt(const Napi::CallbackInfo& info) {
      return Napi::Boolean::New(this->env, this->adaptee > ClassAdapter<T, A>(info).adaptee);
    }
    virtual inline Napi::Value le(const Napi::CallbackInfo& info) {
      return Napi::Boolean::New(this->env, this->adaptee <= ClassAdapter<T, A>(info).adaptee);
    }
    virtual inline Napi::Value ge(const Napi::CallbackInfo& info) {
      return Napi::Boolean::New(this->env, this->adaptee >= ClassAdapter<T, A>(info).adaptee);
    }
};
// TODO:
// make robot->MemoryRegion.New(env) work as MemoryRegion::New(env, adaptee = nullptr), and also allow the {} syntax too.
// and also the ctor as MemoryRegion::Ctor(env) and an instanceof checker named MemoryRegion::IsInstance(Napi::Value value)
