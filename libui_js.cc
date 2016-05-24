#include <node.h>
#include "window.h"

namespace libui {

using v8::Local;
using v8::Object;

void InitAll(Local<Object> exports) {
  Window::Init(exports);
}

NODE_MODULE(addon, InitAll) // no semicolon here

} // namespace libui
