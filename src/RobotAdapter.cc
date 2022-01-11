#include "RobotAdapter.h"
#include "BoundsAdapter.h"
#include "ClipboardAdapter.h"
#include "ColorAdapter.h"
#include "HashAdapter.h"
#include "ImageAdapter.h"
#include "KeyboardAdapter.h"
#include "MemoryAdapter.h"
#include "ModuleAdapter.h"
#include "PointAdapter.h"
#include "RangeAdapter.h"
#include "ScreenAdapter.h"
#include "SizeAdapter.h"
#include "TimerAdapter.h"
#include "WindowAdapter.h"

RobotAdapter::RobotAdapter(Napi::Env env, Napi::Object exports) {
  DefineAddon(exports, {
    InstanceValue("sleep", Napi::Function::New(env, &TimerAdapter::sleep)),
    InstanceValue("clock", Napi::Function::New(env, &TimerAdapter::getCpuTime)),
    InstanceValue("Bounds", BoundsAdapter::GetConstructor(env)),
    InstanceValue("Clipboard", ClipboardAdapter::Init(env)),
    InstanceValue("Color", ColorAdapter::GetConstructor(env)),
    InstanceValue("Hash", HashAdapter::GetConstructor(env)),
    InstanceValue("Image", ImageAdapter::GetConstructor(env)),
    InstanceValue("Keyboard", KeyboardAdapter::GetConstructor(env)),
    InstanceValue("Memory", MemoryAdapter::GetConstructor(env)),
    InstanceValue("Module", ModuleAdapter::GetConstructor(env)),
    InstanceValue("Point", PointAdapter::GetConstructor(env)),
    InstanceValue("Range", RangeAdapter::GetConstructor(env)),
    InstanceValue("Screen", ScreenAdapter::GetConstructor(env)),
    InstanceValue("Size", SizeAdapter::GetConstructor(env)),
    InstanceValue("Timer", TimerAdapter::GetConstructor(env)),
    InstanceValue("Window", WindowAdapter::GetConstructor(env)),
  });
}

NODE_API_ADDON(RobotAdapter);
