#include <iostream>
#include "window.h"

namespace libui {

using v8::Context;
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

Persistent<Function> Window::constructor;

Window::Window() {
}

Window::~Window() {
}

static uiWindow *mainwin;
static Isolate* isolate;
static Local<Function> onClosingCb;

static int onClosing(uiWindow* window, void* data) {
  const unsigned argc = 0;
  Local<Value> argv[argc] = { };
  onClosingCb->Call(Null(isolate), argc, argv);
  uiControlDestroy(uiControl(window));
  uiQuit();
  return 0;
}

void Window::Init(Local<Object> exports) {
  Isolate* isolate = exports->GetIsolate();

  // Prepare constructor template
  Local<FunctionTemplate> tpl = FunctionTemplate::New(isolate, New);
  tpl->SetClassName(String::NewFromUtf8(isolate, "Window"));
  tpl->InstanceTemplate()->SetInternalFieldCount(1);

  // Prototype
  NODE_SET_PROTOTYPE_METHOD(tpl, "setChild", SetChild);

  constructor.Reset(isolate, tpl->GetFunction());
  exports->Set(String::NewFromUtf8(isolate, "Window"),
               tpl->GetFunction());
}

void Window::New(const FunctionCallbackInfo<Value>& args) {
  // global isolate to be used for onClosing
  isolate = args.GetIsolate();

  if (args.IsConstructCall()) {
    // Invoked as constructor: `new MyObject(...)`
    bool optionsPassed = !args[0]->IsUndefined() && args[0]->IsObject();

    Local<Object> options;
    char *title = "";
    int width = 512;
    int height= 512;
    bool hasMenu = true;
    bool margined = true;
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
      hasMenu = _hasMenu->IsBoolean() ? _hasMenu->BooleanValue() : hasMenu;

      Local<Value> _onClosingCb = options->Get(String::NewFromUtf8(isolate, "onClosing"));
      if (_onClosingCb->IsFunction()) {
        onClosingCb = Local<Function>::Cast(_onClosingCb);
      }

      Local<Value> _margined = options->Get(String::NewFromUtf8(isolate, "margined"));
      margined = _margined->IsBoolean() ? _margined->BooleanValue() : margined;
    }

    mainwin = uiNewWindow(title, width, height, hasMenu);
    uiWindowOnClosing(mainwin, onClosing, NULL);
    uiWindowSetMargined(mainwin, margined);
    uiControlShow(uiControl(mainwin));

    Window* obj = new Window();
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

void Window::SetChild(const FunctionCallbackInfo<Value>& args) {
  Isolate* isolate = args.GetIsolate();

  Window* obj = ObjectWrap::Unwrap<Window>(args.Holder());
  args.GetReturnValue().Set(Number::New(isolate, 1));
}


} // namespace libui
