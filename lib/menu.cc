#include "libui_js.h"

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

static int shouldQuit(void *data) {
  int* index = (int*)data; // just casting void* to int*
  Persistent<Function> onClickCb(isolate, onClickCallbacks[*index]);
  const unsigned argc = 0;
  Local<Value> argv[argc] = {};
  Local<Function>::New(isolate, onClickCb)->Call(Null(isolate), argc, argv);
  //uiControlDestroy(uiControl(mainwin));
  return 1;
}

void itemClicked(uiMenuItem* item, uiWindow* w, void* data) {
  int* index = (int*)data; // just casting void* to int*
  Persistent<Function> onClickCb(isolate, onClickCallbacks[*index]);
  const unsigned argc = 0;
  Local<Value> argv[argc] = {};
  Local<Function>::New(isolate, onClickCb)->Call(Null(isolate), argc, argv);
}

bool isMember(vector<string> legitVector, string test) {
  return std::find(legitVector.begin(), legitVector.end(), test) != legitVector.end();
}

Menu::Menu(char* name, Local<Value> items_) {
  static uiMenu* menu;
  char* label;
  const char* type;
  menu = uiNewMenu(name);

  vector<string> allTypes {"item", "quit", "preferences", "about", "check"};
  vector<string> noOnClick {"quit"};
  vector<string> noLabel {"quit", "preferences", "about"};

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

      Local<Value> _type = itemObj->Get(String::NewFromUtf8(isolate, "type"));
      if (_type->IsString()) {
        type = ToCString(_type);
      } else {
        string s = "item";
        type = s.c_str(); // defaults to item
      }
      string type_str(type);
      bool legitType = isMember(allTypes, type_str);

      std::stringstream ss;
      for(unsigned int i=0; i<allTypes.size(); ++i) {
        if (i == 0) {
          ss << allTypes[0];
          continue;
        }
        ss << ", " <<  allTypes[i];
      }
      string errorMsg("Menu item must be either {"+ss.str()+"}");

      if (!legitType) {
        isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, errorMsg.c_str())));
        return;
      }

      Local<Value> _label = itemObj->Get(String::NewFromUtf8(isolate, "label"));
      bool legitLabel = !isMember(noLabel, type_str);
      if (_label->IsString() && legitLabel) {
        label = ToCString(_label);
      } else if (!_label->IsString() && legitLabel) {
        string s("Label must be provided for "+type_str+" items");
        isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, s.c_str())));
        return;
      }

      uiMenuItem *item;
      if (type_str == "item") {
        item = uiMenuAppendItem(menu, label);
      } else if (type_str == "preferences") {
        item = uiMenuAppendPreferencesItem(menu);
      } else if (type_str == "quit") {
        item = uiMenuAppendQuitItem(menu);
      } else if (type_str == "check") {
        item = uiMenuAppendCheckItem(menu, label);
      } else if (type_str == "about") {
        item = uiMenuAppendAboutItem(menu);
      } else {
        isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, errorMsg.c_str())));
        return;
      }

      Local<Value> _onClickCb = itemObj->Get(String::NewFromUtf8(isolate, "onClick"));
      if (!_onClickCb->IsFunction()) {
        const char* msg = ("onClick in menu item ("
                           +type_str+
                           ") must be a function").c_str();
        isolate->ThrowException(Exception::TypeError(
          String::NewFromUtf8(isolate, msg)));
        return;
      } else {
        Local<Function> loc = Local<Function>::Cast(_onClickCb);
        Persistent<Function, CopyablePersistentTraits<Function>> onClickCb(isolate, loc);
        onClickCallbacks[i] = onClickCb;
        
        int* ptr = (int*)malloc(sizeof(int));
        *ptr = i;

        if (type_str == "quit") {
          uiOnShouldQuit(shouldQuit, ptr); 
        } else {
          uiMenuItemOnClicked(item, itemClicked, ptr);
        }
      }
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
