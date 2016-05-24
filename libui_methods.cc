#include <cstring>
#include "libui_methods.h"

namespace libui {

using v8::Exception;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::String;
using v8::Value;

void LibUiInit(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed.
  if (args.Length() > 0) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  uiInitOptions o;
  const char *err;

  memset(&o, 0, sizeof (uiInitOptions));
  err = uiInit(&o);
  if (err != NULL) {
    uiFreeInitError(err);
    isolate->ThrowException(Exception::Error(
      String::NewFromUtf8(isolate, err)));
  }
}

void LibUiRender(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  // Check the number of arguments passed.
  if (args.Length() > 0) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }
  uiMain();
  uiUninit();
}

} // namespace libui
