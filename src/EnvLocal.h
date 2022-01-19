#include <napi.h>
#include <unordered_map>

/**
 * Keeps a per-env copy of a Napi::Object
 * Useful for thread-safe saving of JS constructors.
 */
class EnvLocal {

  private:
    class MapEntry {
      public:
        EnvLocal *env_local;
        napi_env env;
        napi_ref ref;
    };
    std::unordered_map<napi_env, MapEntry> map;
    static void env_cleanup_hook(MapEntry *entry);

  public:
    ~EnvLocal();
    void set(Napi::Object obj);
    void erase(napi_env env);
    bool has(napi_env env);
    Napi::Object get(napi_env env);
};
