#include <nan.h>
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


char* ToCString(Local<v8::Value> value) {
  String::Utf8Value utf8Value(value);
  char *str = (char *) malloc(utf8Value.length() + 1);
  strcpy(str, *utf8Value);
  return str;
}

void CreateVerticalBox(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();
  args.GetReturnValue().Set(String::NewFromUtf8(isolate, "hello world"));
}

static uiWindow *mainwin;
static Isolate* isolate;
static Local<Function> onClosingCb;

void Noop(const FunctionCallbackInfo<Value>& args) {
}

static int onClosing(uiWindow* window, void* data) {
  const unsigned argc = 0;
  Local<Value> argv[argc] = { };
  onClosingCb->Call(Null(isolate), argc, argv);
  uiControlDestroy(uiControl(window));
  uiQuit();
  return 0;
}

// new Window([options], cb)
void CreateWindow(const FunctionCallbackInfo<Value>& args) {
  isolate = args.GetIsolate();

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
  cb = optionsPassed ? Local<Function>::Cast(args[1]) : Local<Function>::Cast(args[0]);
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
  onClosingCb = FunctionTemplate::New(isolate, Noop)->GetFunction(); // noop

  if (optionsPassed) {
    options = args[0]->ToObject();

    Local<Value> _title = options->Get(String::NewFromUtf8(isolate, "title"));
    title = _title->IsString() ? ToCString(_title) : title;

    Local<Value> _width = options->Get(String::NewFromUtf8(isolate, "width"));
    width = _width->IsNumber() ? _width->NumberValue() : width;

    Local<Value> _height = options->Get(String::NewFromUtf8(isolate, "height"));
    height = _height->IsNumber() ? _height->NumberValue() : height;

    Local<Value> _hasMenu = options->Get(String::NewFromUtf8(isolate, "hasMenu"));
    hasMenu = _hasMenu->IsNumber() ? _hasMenu->BooleanValue() : hasMenu;

    Local<Value> _onClosingCb = options->Get(String::NewFromUtf8(isolate, "onClosing"));
    if (_onClosingCb->IsFunction()) {
      onClosingCb = Local<Function>::Cast(_onClosingCb);
    }
  }

  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, CreateVerticalBox);
  Local<Function> VerticalBoxFn = tpl->GetFunction();
  VerticalBoxFn->SetName(String::NewFromUtf8(isolate, "VerticalBox"));

  Local<Object> windowObj = Object::New(isolate);
  //windowObj->Set(String::NewFromUtf8(isolate, "VerticalBox"), VerticalBoxFn);

  Local<Value> argv[argc] = { Null(isolate), windowObj };
  cb->Call(Null(isolate), argc, argv);

  mainwin = uiNewWindow(title, width, height, hasMenu);
  uiWindowOnClosing(mainwin, onClosing, NULL);

  uiControlShow(uiControl(mainwin));
  uiMain();
  uiUninit();

  return;
}

void init(Local<Object> exports) {
  NODE_SET_METHOD(exports, "Window", CreateWindow);
}

NODE_MODULE(liblibui, init) // no semicolon here
