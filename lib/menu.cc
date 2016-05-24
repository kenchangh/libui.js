#include <iostream>
#include <string>
#include "menu.h"

namespace libui {

using std::string;
using v8::Array;
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

static Isolate* isolate;
static Local<Function> onClickCb;

void Menu::ItemClicked(uiMenuItem *item, uiWindow *w, void *data) {
  const unsigned argc = 0;
  Local<Value> argv[argc] = {};
  onClickCb->Call(Null(isolate), argc, argv);
}

Menu::Menu(char* name, Local<Value> items_) {
  uiMenu* menu;
  static uiMenuItem *item;
  char* label;
  char* type;
  Local<Function> onClick;
  menu = uiNewMenu(name);

  Local<Array> items;
  if (items_->IsArray()) {
    items = Local<Array>::Cast(items_);
    for (unsigned int i = 0; i < items->Length(); i++) {
      Local<Value> itemValue = items->Get(i);
      if (!itemValue->IsObject()) {
        isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Menu item must be an object")));
        return;
      }
      Local<Object> itemObj = itemValue->ToObject();

      Local<Value> _label = itemObj->Get(String::NewFromUtf8(isolate, "label"));
      if (!_label->IsString()) {
        isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Menu item's label must be string")));
        return;
      }
      label = ToCString(_label);

      Local<Value> _type = itemObj->Get(String::NewFromUtf8(isolate, "type"));
      if (!_type->IsString()) {
        isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Menu item's type must be string")));
        return;
      }
      string type = ToCString(_type);

      Local<Value> _onClickCb = itemObj->Get(String::NewFromUtf8(isolate, "onClick"));
      if (!_onClickCb->IsFunction()) {
        isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, "Menu item's onClick must be a function")));
        return;
      }
      onClickCb = Local<Function>::Cast(_onClickCb);

      item = uiMenuAppendItem(menu, label);
      uiMenuItemOnClicked(item, Menu::ItemClicked, NULL);
    }
  }
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
  isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    if (args[0]->IsUndefined()) {
      isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Menu name cannot be undefined")));
      return;
    }

    char* name = ToCString(args[0]);
    //Menu* obj = new Menu(name, args[1]);
    Menu* obj = new Menu(name, args[1]);
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
