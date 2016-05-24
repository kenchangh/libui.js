#include <iostream>
#include "menu.h"

namespace libui {

using v8::Context;
using v8::Exception;
using v8::Function;
using v8::FunctionCallbackInfo;
using v8::FunctionTemplate;
using v8::Isolate;
using v8::Local;
using v8::Number;
using v8::Object;
using v8::Persistent;
using v8::String;
using v8::Value;

Persistent<Function> Menu::constructor;

Menu::Menu(char* name) : name(name) {
  uiMenu* menu;
  uiMenuItem *item;
  menu = uiNewMenu(name);
}
Menu::~Menu() {
}

void Menu::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Menu"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "appendItem", AppendItem);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Menu"),
               tpl->GetFunction());
}

void Menu::New(const FunctionCallbackInfo<Value>& args) {
  // global isolate to be used for onClosing
  Isolate* isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    if (args[0]->IsUndefined()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Menu name cannot be undefined")));
      return;
    }

    char* name = ToCString(args[0]);
    Menu* obj = new Menu(name);
    obj->Wrap(args.This());
    args.GetReturnValue().Set(args.This());
  } else {
    // Invoked as plain function `MyObject(...)`, turn into construct call.
    const int argc = 1;
    Local<Value> argv[argc] = { args[0] };
    Local<Context> context = isolate->GetCurrentContext();
    Local<Function> cons = Local<Function>::New(isolate, constructor);
    Local<Object> result =
        cons->NewInstance(context, argc, argv).ToLocalChecked();
    args.GetReturnValue().Set(result);
  }
}

void Menu::AppendItem(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Menu* obj = ObjectWrap::Unwrap<Menu>(args.Holder());
  args.GetReturnValue().Set(Number::New(isolate, 1));
}


} // namespace libui
