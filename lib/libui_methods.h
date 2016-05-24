#ifndef LIBUI_METHODS_H
#define LIBUI_METHODS_H

#include "node.h"
#include "../libui/ui.h"

namespace libui {

using v8::FunctionCallbackInfo;
using v8::Value;

void LibUiInit(const FunctionCallbackInfo<Value>& args);
void LibUiRender(const FunctionCallbackInfo<Value>& args);

} // namespace libui

#endif
