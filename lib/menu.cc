#include <vector>
#include <iostream>
#include <unordered_map>
#include <string>
#include "menu.h"

namespace libui {

using std::cout;
using std::vector;
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
using v8::CopyablePersistentTraits;

Persistent<Function> Menu::constructor;

static vector<Persistent<Function, CopyablePersistentTraits<Function>>> onClickCallbacks;
static Isolate* isolate;

void itemClicked(uiMenuItem* item, uiWindow* w, void* data) {
  int* index = (int*)data; // just casting void* to int*
  Persistent<Function> onClickCb(isolate, onClickCallbacks[*index]);
  const unsigned argc = 0;
  Local<Value> argv[argc] = {};
  Local<Function>::New(isolate, onClickCb)->Call(Null(isolate), argc, argv);
}

Menu::Menu(char* name, Local<Value> items_) {
  static uiMenu* menu;
  char* label;
  char* type;
  menu = uiNewMenu(name);

  Local<Array> items;
  if (items_->IsArray()) {
    items = Local<Array>::Cast(items_);
    int len = items->Length();
    onClickCallbacks.resize(len);

    for (int i = 0; i < len; i++) {
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
      Local<Function> loc = Local<Function>::Cast(_onClickCb);
      Persistent<Function, CopyablePersistentTraits<Function>> onClickCb(isolate, loc);
      onClickCallbacks[i] = onClickCb;

      uiMenuItem *item;
      int* ptr = (int*)malloc(sizeof(int));
      *ptr = i;
      item = uiMenuAppendItem(menu, label);
      uiMenuItemOnClicked(item, itemClicked, ptr);
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
}


} // namespace libui
