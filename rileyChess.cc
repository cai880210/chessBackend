#include <napi.h>
#include "main.h"

Napi::String jsGetNextMove(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  std::string arg0 = info[0].ToString();
  return Napi::String::New(env, getNextMove(arg0));
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "jsGetNextMove"), Napi::Function::New(env, jsGetNextMove));
  return exports;
}

NODE_API_MODULE(addon, Init)
