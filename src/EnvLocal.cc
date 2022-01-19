#include "EnvLocal.h"

void EnvLocal::env_cleanup_hook(EnvLocal::MapEntry *entry) {
  entry->env_local->map.erase(entry->env);
}

EnvLocal::~EnvLocal() {
  for(auto it : map) {
    this->erase(it.first);
  }
}

void EnvLocal::set(Napi::Object obj) {
  napi_env env = obj.Env();
  this->erase(env);
  map[env] = {this, env, nullptr};
  auto r = Napi::ObjectReference::New(obj, 1);
  NAPI_THROW_IF_FAILED_VOID(env,
    napi_create_reference(env, (napi_value)obj, 1, &map[env].ref)
  );
  NAPI_THROW_IF_FAILED_VOID(env,
    napi_add_env_cleanup_hook(env, (void (*)(void *))&EnvLocal::env_cleanup_hook, &map[env])
  );
}

void EnvLocal::erase(napi_env env) {
  if(this->has(env)) {
    napi_remove_env_cleanup_hook(env, (void (*)(void *))&EnvLocal::env_cleanup_hook, &map[env]);
    napi_delete_reference(env, map[env].ref);
    map.erase(env);
  }
}

bool EnvLocal::has(napi_env env) {
  return map.count(env) > 0;
}

Napi::Object EnvLocal::get(napi_env env) {
  napi_value value;
  NAPI_THROW_IF_FAILED(env,
    napi_get_reference_value(env, map.at(env).ref, &value),
    Napi::Object()
  );
  return Napi::Object(env, value);
}
