// myobject.h
#ifndef UTILS_H
#define UTILS_H

#include <node.h>
#include <stdlib.h>
#include <cstring>

namespace libui {

using v8::Local;
using v8::Value;
using v8::FunctionCallbackInfo;

char* ToCString(Local<v8::Value> value);
void Noop(const FunctionCallbackInfo<Value>& args);

} // namespace libui

#endif
