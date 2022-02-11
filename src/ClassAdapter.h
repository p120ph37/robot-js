#pragma once

#include <napi.h>
#include <unordered_map>

namespace {
  const char* CONSTRUCTOR = "constructor";
  const char* ADAPTEE_SYMBOL = "adaptee_symbol";
}

template <typename T, typename A>
class ClassAdapter : public Napi::ObjectWrap<T> {

  private:
    static std::unordered_map<napi_env, Napi::ObjectReference> env_local;

  protected:
    Napi::Env env;
    // Utility function to obtain a private Napi::Object local to each env, in which to store things.
    inline static Napi::Object EnvLocal(napi_env env) {
      if(!env_local.count(env)) {
        auto o = Napi::Object::New(env);
        o[CONSTRUCTOR] = T::Init(env);
        o[ADAPTEE_SYMBOL] = Napi::Symbol::New(env, ADAPTEE_SYMBOL);
        env_local[env] = Napi::Persistent(o);
        Napi::Env(env).AddCleanupHook([env] {
          env_local.erase(env);
        });
      }
      return env_local[env].Value();
    }
    // Utility function for use in constructor to allow retrieval of raw adaptee if passed internally.
    inline bool WrapAdaptee(const Napi::CallbackInfo& info) {
      if(EnvLocal(env).Get(ADAPTEE_SYMBOL).StrictEquals(info[0]) && info[1].IsExternal()) {
        adaptee = *info[1].As<Napi::External<A>>().Data();
        return true;
      }
      return false;
    }
    // Utility function for use in constructor to initilize as a copy of another object of the same class
    inline bool CopyThat(const Napi::CallbackInfo& info) {
      if(T::IsInstance(info[0])) {
        adaptee = T::Unwrap(info[0])->adaptee;
        return true;
      }
      return false;
    }
    // Utility function for use in constructor to initialize via default constructor 
    inline bool ConstructDefault(const Napi::CallbackInfo& info) {
      if(info[0].IsUndefined()) {
        adaptee = A();
        return true;
      }
      return false;
    }

  public:
    A adaptee;
//    static Napi::Function Init(napi_env env);
    inline static Napi::Function GetConstructor(napi_env env) {
      return EnvLocal(env).Get(CONSTRUCTOR).As<Napi::Function>();
    }
    // Will only work for classes which use `if(WrapAdaptee(info)) return;` in their constructor:
    inline static Napi::Value New(napi_env env, A const &adaptee) {
      return New(env, {
        EnvLocal(env).Get(ADAPTEE_SYMBOL).As<Napi::Symbol>(),
        Napi::External<A>::New(env, (A*)&adaptee)
      });
    }
    // Shortcuts to avoid the verbosity of T::GetConstructor(info.Env()).New({info[0], info[1], info[2] ... })
    inline static Napi::Value New(const Napi::CallbackInfo& info) {
      std::vector<napi_value> args;
      for(size_t i = 0; i < info.Length(); i++) args.push_back(info[i]);
      return New(info.Env(), args);
    }
    inline static Napi::Value New(napi_env env, const std::vector<napi_value>& args) {
      return GetConstructor(env).New(args);
    }
    inline static Napi::Value New(napi_env env, const std::initializer_list<napi_value> &args) {
      return GetConstructor(env).New(args);
    }
    // Go through the motions of creating a new JS object (including all ObjectWrap constructor logic),
    // then strip away the JS parts, leaving just the constructed adaptee.
    inline static A NewAdaptee(const Napi::CallbackInfo& info) {
      Napi::HandleScope scope(info.Env());
      return T::Unwrap(T::New(info))->adaptee;
    }
    inline static A NewAdaptee(napi_env env, const std::vector<napi_value>& args) {
      Napi::HandleScope scope(env);
      return T::Unwrap(T::New(env, args))->adaptee;
    }
    inline static A NewAdaptee(napi_env env, const std::initializer_list<napi_value> &args) {
      Napi::HandleScope scope(env);
      return T::Unwrap(T::New(env, args))->adaptee;
    }
    // Check if the argument is a JS instance of this class
    inline static bool IsInstance(Napi::Value value) {
      return value.IsObject() && value.As<Napi::Object>().InstanceOf(GetConstructor(value.Env()));
    }
    // An "unwrap" variant which assumes the argument is an object
    inline static T* Unwrap(Napi::Value obj) {
      return Napi::ObjectWrap<T>::Unwrap(obj.As<Napi::Object>());
    }
    inline ClassAdapter(const Napi::CallbackInfo& info) :
      Napi::ObjectWrap<T>(info),
      env(info.Env()) {}
    // Change some ObjectWrap behavior to add auto-constructor mode
    inline static Napi::Value OnCalledAsFunction(const Napi::CallbackInfo& info) {
      return New(info);
    }

    // Mixin of eq/ne methods which do type-strict comparisons.
    class StrictEq {
      public:
        inline Napi::Value eq(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee == T::Unwrap(info[0])->adaptee);
        }
        inline Napi::Value ne(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee != T::Unwrap(info[0])->adaptee);
        }
    };
    // Mixin of eq/ne methods which use the contructor to do loose comparisons.
    class Eq {
      public:
        inline Napi::Value eq(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee == T::NewAdaptee(info));
        }
        inline Napi::Value ne(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee != T::NewAdaptee(info));
        }
    };
    // Mixin of lt/gt/le/ge methods which use the contructor to do loose comparisons.
    class Cmp {
      public:
        inline Napi::Value lt(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee < T::NewAdaptee(info));
        }
        inline Napi::Value gt(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee > T::NewAdaptee(info));
        }
        inline Napi::Value le(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee <= T::NewAdaptee(info));
        }
        inline Napi::Value ge(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee >= T::NewAdaptee(info));
        }
    };
    // Mixin of lt/gt/le/ge methods which do type-strict comparisons.
    class StrictCmp {
      public:
        inline Napi::Value lt(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee < T::Unwrap(info[0])->adaptee);
        }
        inline Napi::Value gt(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee > T::Unwrap(info[0])->adaptee);
        }
        inline Napi::Value le(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee <= T::Unwrap(info[0])->adaptee);
        }
        inline Napi::Value ge(const Napi::CallbackInfo& info) {
          return Napi::Boolean::New(static_cast<T*>(this)->env, static_cast<T*>(this)->adaptee >= T::Unwrap(info[0])->adaptee);
        }
    };

};

template <typename T, typename A>
std::unordered_map<napi_env, Napi::ObjectReference> ClassAdapter<T, A>::env_local = {};

// TODO:
// Finish the MemoryAdapter types implementation
// make robot->MemoryRegion.New(env) work as MemoryRegion::New(env, adaptee = nullptr), and also allow the {} syntax too.
// and also the ctor as MemoryRegion::Ctor(env) and an instanceof checker named MemoryRegion::IsInstance(Napi::Value value)

// implement the clone and compare functions for various classes (from robot.js)