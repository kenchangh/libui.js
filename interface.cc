#include <nan.h>
#include <iostream>
#include <stdlib.h>
#include <cstring>
#include <string>
#include "libui/ui.h"
#include <typeinfo>

using std::memset;

using v8::Exception;
using v8::Function;
using v8::FunctionTemplate;
using v8::FunctionCallbackInfo;
using v8::Isolate;
using v8::Local;
using v8::Null;
using v8::Object;
using v8::String;
using v8::Number;
using v8::Value;


static uiWindow *mainwin;

char* ToCString(String::Utf8Value& value) {
  return *value;
}

void CreateVerticalBox(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "hello world"));
}

// new Window([options], cb)
void CreateWindow(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  if (args.Length() < 1) {
    // Throw an Error that is passed back to JavaScript
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Wrong number of arguments")));
    return;
  }

  if (args.Length() == 1 && !args[0]->IsFunction()) {
    isolate->ThrowException(Exception::TypeError(
        String::NewFromUtf8(isolate, "Callback not provided")));
    return;
  }

  Local<Function> cb;
  const bool optionsPassed = !args[0]->IsFunction();
  cb = optionsPassed ? Local<Function>::Cast(args[0]) : Local<Function>::Cast(args[1]);
  const unsigned argc = 2;

  uiInitOptions libuiInitOptions;
  memset(&libuiInitOptions, 0, sizeof (libuiInitOptions));
  const char *err;

  // initiate UI and do error callback
  err = uiInit(&libuiInitOptions);
  if (err != NULL) { // <string>
    //fprintf(stderr, "error initializing ui: %s\n", err);
    uiFreeInitError(err);

    Local<Value> argv[argc] = { String::NewFromUtf8(isolate, err), Null(isolate) };
    cb->Call(Null(isolate), argc, argv);
    return;
  }

  Local<Object> options;
  char *title = "";
  int width = 512;
  int height= 512;
  bool hasMenu = true;

  if (optionsPassed) {
    options = args[0]->ToObject();

    Local<Value> _title = options->Get(String::NewFromUtf8(isolate, "title"));
    String::Utf8Value str(_title);
    title = _title->IsString() ? ToCString(str) : title;

    Local<Value> _width = options->Get(String::NewFromUtf8(isolate, "width"));
    width = _width->IsNumber() ? _width->NumberValue() : width;

    Local<Value> _height = options->Get(String::NewFromUtf8(isolate, "height"));
    height = _height->IsNumber() ? _height->NumberValue() : height;

    Local<Value> _hasMenu = options->Get(String::NewFromUtf8(isolate, "hasMenu"));
    hasMenu = _hasMenu->IsNumber() ? _hasMenu->BooleanValue() : hasMenu;
  }

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, CreateVerticalBox);
  Local<Function> VerticalBoxFn = tpl->GetFunction();
  VerticalBoxFn->SetName(String::NewFromUtf8(isolate, "VerticalBox"));

  Local<Object> windowObj = Object::New(isolate);
  //windowObj->Set(String::NewFromUtf8(isolate, "VerticalBox"), VerticalBoxFn);

  Local<Value> argv[argc] = { Null(isolate), windowObj };
  cb->Call(Null(isolate), argc, argv);

  mainwin = uiNewWindow(title, width, height, hasMenu);

  uiControlShow(uiControl(mainwin));
  uiMain();
  uiUninit();

  return;
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "Window", CreateWindow);
}

NODE_MODULE(liblibui, init) // no semicolon here
