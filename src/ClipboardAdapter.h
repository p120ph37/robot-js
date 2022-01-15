#include <napi.h>

namespace ClipboardAdapter {

  Napi::Object Init(Napi::Env env);

  Napi::Value clear(const Napi::CallbackInfo& info);
  Napi::Value hasText(const Napi::CallbackInfo& info);
  Napi::Value getText(const Napi::CallbackInfo& info);
  Napi::Value setText(const Napi::CallbackInfo& info);
  Napi::Value hasImage(const Napi::CallbackInfo& info);
  Napi::Value getImage(const Napi::CallbackInfo& info);
  Napi::Value setImage(const Napi::CallbackInfo& info);
  Napi::Value getSequence(const Napi::CallbackInfo& info);

};
