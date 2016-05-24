// myobject.h
#ifndef MENU_H
#define MENU_H

#include <node.h>
#include <node_object_wrap.h>
#include "../libui/ui.h"
#include "utils.h"

namespace libui {

class Menu : public node::ObjectWrap {
 public:
  static void Init(v8::Local<v8::Object> exports);

 private:
  explicit Menu(char* menu);
  ~Menu();

  static void New(const v8::FunctionCallbackInfo<v8::Value>& args);
  static void AppendItem(const v8::FunctionCallbackInfo<v8::Value>& args);
  static v8::Persistent<v8::Function> constructor;
  char* name;
};

} // namespace libui

#endif
