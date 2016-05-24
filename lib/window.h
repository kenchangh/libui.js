// myobject.h
#ifndef WINDOW_H
#define WINDOW_H

#include <node.h>
#include <node_object_wrap.h>
#include "../libui/ui.h"
#include "utils.h"

namespace libui {

class Window : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit Window();
  ~Window();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SetChild(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void OpenFile(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void SaveFile(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void MsgBox(const v8::FunctionCallbackInfo<v8::Value>& args);
  static v8::Persistent<v8::Function> constructor;
};

} // namespace libui

#endif
