#pragma once

#include <napi.h>
#include <unordered_map>

/**
 * Keeps a per-env copy of a Napi::Value (or subclass thereof).
 * Useful for thread-safe saving of JS constructors.
 */
template <typename T>
class EnvLocal {

  private:
    class MapEntry {
      public:
        EnvLocal *env_local;
        napi_env env;
        napi_ref ref;
    };
    std::unordered_map<napi_env, MapEntry> map = {};
    static void env_cleanup_hook(MapEntry *entry) {
      napi_delete_reference(entry->env, entry->ref);
      entry->env_local->map.erase(entry->env);
    }

  public:
    ~EnvLocal() {
      for(auto it : map) {
        this->erase(it.first);
      }
    }
    inline void set(T value) {
      napi_env env = value.Env();
      this->erase(env);
      napi_ref ref;
      napi_create_reference(env, value, 1, &ref);
      map[env] = {this, env, ref};
      napi_add_env_cleanup_hook(env, (void (*)(void *))&EnvLocal::env_cleanup_hook, &map[env]);
    }
    inline void erase(napi_env env) {
      if(this->has(env)) {
        napi_remove_env_cleanup_hook(env, (void (*)(void *))&EnvLocal::env_cleanup_hook, &map.at(env));
        map.erase(env);
      }
    }
    inline bool has(napi_env env) {
      return map.count(env) > 0;
    }
    inline T get(napi_env env) {
      napi_value value;
      napi_get_reference_value(env, map.at(env).ref, &value);
      return T(env, value);
    }

};
