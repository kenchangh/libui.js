#include "utils.h"

namespace libui {

using v8::Local;
using v8::String;
using v8::Value;
using v8::FunctionCallbackInfo;

char* ToCString(Local<v8::Value> value) {
  String::Utf8Value utf8Value(value);
  char *str = (char *) malloc(utf8Value.length() + 1);
  strcpy(str, *utf8Value);
  return str;
}

void Noop(const FunctionCallbackInfo<Value>& args) {
}


} // namespace libui
