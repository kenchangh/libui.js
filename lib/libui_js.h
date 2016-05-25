#ifndef LIBUI_JS_H
#define LIBUI_JS_H

#include <stdlib.h>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>

#include <node.h>
#include <node_object_wrap.h>
#include "../libui/ui.h"

namespace libui {

using v8::Local;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::Value;
using v8::Persistent;

char* ToCString(Local<v8::Value> value);
void Noop(const FunctionCallbackInfo<Value>& args);

void LibUiInit(const FunctionCallbackInfo<Value>& args);
void LibUiRender(const FunctionCallbackInfo<Value>& args);

class Menu : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit Menu(char* menu, Local<Value> items);
  ~Menu();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void AppendItem(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void ItemClicked(uiMenuItem *item, uiWindow *w, void *data);
  static v8::Persistent<v8::Function> constructor;
};

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
