#include "RobotAdapter.h"
#include "BoundsAdapter.h"
#include "ClipboardAdapter.h"
#include "ColorAdapter.h"
#include "HashAdapter.h"
#include "ImageAdapter.h"
#include "KeyboardAdapter.h"
#include "MemoryAdapter.h"
#include "ModuleAdapter.h"
#include "MouseAdapter.h"
#include "PointAdapter.h"
#include "ProcessAdapter.h"
#include "RangeAdapter.h"
#include "ScreenAdapter.h"
#include "SizeAdapter.h"
#include "TimerAdapter.h"
#include "WindowAdapter.h"

RobotAdapter::RobotAdapter(Napi::Env env, Napi::Object exports) {
  DefineAddon(exports, {
    InstanceValue("ROBOT_VERSION", Napi::Number::New(env, ROBOT_VERSION)),
    InstanceValue("ROBOT_VERSION_STR", Napi::String::New(env, ROBOT_VERSION_STR " (" ADDON_VERSION_STR ")")),
    InstanceValue("ADDON_VERSION", Napi::Number::New(env, ADDON_VERSION)),
    InstanceValue("ADDON_VERSION_STR", Napi::String::New(env, ADDON_VERSION_STR)),

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
    InstanceValue("Mouse", MouseAdapter::GetConstructor(env)),
    InstanceValue("Point", PointAdapter::GetConstructor(env)),
    InstanceValue("Process", ProcessAdapter::GetConstructor(env)),
    InstanceValue("Range", RangeAdapter::GetConstructor(env)),
    InstanceValue("Screen", ScreenAdapter::GetConstructor(env)),
    InstanceValue("Size", SizeAdapter::GetConstructor(env)),
    InstanceValue("Timer", TimerAdapter::GetConstructor(env)),
    InstanceValue("Window", WindowAdapter::GetConstructor(env)),

    InstanceValue("KEY_SPACE", Napi::Number::New(env, Robot::Key::KeySpace)),
    InstanceValue("KEY_ESCAPE", Napi::Number::New(env, Robot::Key::KeyEscape)),

    InstanceValue("KEY_TAB", Napi::Number::New(env, Robot::Key::KeyTab)),
    InstanceValue("KEY_ALT", Napi::Number::New(env, Robot::Key::KeyAlt)),
    InstanceValue("KEY_LALT", Napi::Number::New(env, Robot::Key::KeyLAlt)),
    InstanceValue("KEY_RALT", Napi::Number::New(env, Robot::Key::KeyRAlt)),
    InstanceValue("KEY_CONTROL", Napi::Number::New(env, Robot::Key::KeyControl)),
    InstanceValue("KEY_LCONTROL", Napi::Number::New(env, Robot::Key::KeyLControl)),
    InstanceValue("KEY_RCONTROL", Napi::Number::New(env, Robot::Key::KeyRControl)),
    InstanceValue("KEY_SHIFT", Napi::Number::New(env, Robot::Key::KeyShift)),
    InstanceValue("KEY_LSHIFT", Napi::Number::New(env, Robot::Key::KeyLShift)),
    InstanceValue("KEY_RSHIFT", Napi::Number::New(env, Robot::Key::KeyRShift)),
    InstanceValue("KEY_SYSTEM", Napi::Number::New(env, Robot::Key::KeySystem)),
    InstanceValue("KEY_LSYSTEM", Napi::Number::New(env, Robot::Key::KeyLSystem)),
    InstanceValue("KEY_RSYSTEM", Napi::Number::New(env, Robot::Key::KeyRSystem)),

    InstanceValue("KEY_F1", Napi::Number::New(env, Robot::Key::KeyF1)),
    InstanceValue("KEY_F2", Napi::Number::New(env, Robot::Key::KeyF2)),
    InstanceValue("KEY_F3", Napi::Number::New(env, Robot::Key::KeyF3)),
    InstanceValue("KEY_F4", Napi::Number::New(env, Robot::Key::KeyF4)),
    InstanceValue("KEY_F5", Napi::Number::New(env, Robot::Key::KeyF5)),
    InstanceValue("KEY_F6", Napi::Number::New(env, Robot::Key::KeyF6)),
    InstanceValue("KEY_F7", Napi::Number::New(env, Robot::Key::KeyF7)),
    InstanceValue("KEY_F8", Napi::Number::New(env, Robot::Key::KeyF8)),
    InstanceValue("KEY_F9", Napi::Number::New(env, Robot::Key::KeyF9)),
    InstanceValue("KEY_F10", Napi::Number::New(env, Robot::Key::KeyF10)),
    InstanceValue("KEY_F11", Napi::Number::New(env, Robot::Key::KeyF11)),
    InstanceValue("KEY_F12", Napi::Number::New(env, Robot::Key::KeyF12)),

    InstanceValue("KEY_0", Napi::Number::New(env, Robot::Key::Key0)),
    InstanceValue("KEY_1", Napi::Number::New(env, Robot::Key::Key1)),
    InstanceValue("KEY_2", Napi::Number::New(env, Robot::Key::Key2)),
    InstanceValue("KEY_3", Napi::Number::New(env, Robot::Key::Key3)),
    InstanceValue("KEY_4", Napi::Number::New(env, Robot::Key::Key4)),
    InstanceValue("KEY_5", Napi::Number::New(env, Robot::Key::Key5)),
    InstanceValue("KEY_6", Napi::Number::New(env, Robot::Key::Key6)),
    InstanceValue("KEY_7", Napi::Number::New(env, Robot::Key::Key7)),
    InstanceValue("KEY_8", Napi::Number::New(env, Robot::Key::Key8)),
    InstanceValue("KEY_9", Napi::Number::New(env, Robot::Key::Key9)),

    InstanceValue("KEY_A", Napi::Number::New(env, Robot::Key::KeyA)),
    InstanceValue("KEY_B", Napi::Number::New(env, Robot::Key::KeyB)),
    InstanceValue("KEY_C", Napi::Number::New(env, Robot::Key::KeyC)),
    InstanceValue("KEY_D", Napi::Number::New(env, Robot::Key::KeyD)),
    InstanceValue("KEY_E", Napi::Number::New(env, Robot::Key::KeyE)),
    InstanceValue("KEY_F", Napi::Number::New(env, Robot::Key::KeyF)),
    InstanceValue("KEY_G", Napi::Number::New(env, Robot::Key::KeyG)),
    InstanceValue("KEY_H", Napi::Number::New(env, Robot::Key::KeyH)),
    InstanceValue("KEY_I", Napi::Number::New(env, Robot::Key::KeyI)),
    InstanceValue("KEY_J", Napi::Number::New(env, Robot::Key::KeyJ)),
    InstanceValue("KEY_K", Napi::Number::New(env, Robot::Key::KeyK)),
    InstanceValue("KEY_L", Napi::Number::New(env, Robot::Key::KeyL)),
    InstanceValue("KEY_M", Napi::Number::New(env, Robot::Key::KeyM)),
    InstanceValue("KEY_N", Napi::Number::New(env, Robot::Key::KeyN)),
    InstanceValue("KEY_O", Napi::Number::New(env, Robot::Key::KeyO)),
    InstanceValue("KEY_P", Napi::Number::New(env, Robot::Key::KeyP)),
    InstanceValue("KEY_Q", Napi::Number::New(env, Robot::Key::KeyQ)),
    InstanceValue("KEY_R", Napi::Number::New(env, Robot::Key::KeyR)),
    InstanceValue("KEY_S", Napi::Number::New(env, Robot::Key::KeyS)),
    InstanceValue("KEY_T", Napi::Number::New(env, Robot::Key::KeyT)),
    InstanceValue("KEY_U", Napi::Number::New(env, Robot::Key::KeyU)),
    InstanceValue("KEY_V", Napi::Number::New(env, Robot::Key::KeyV)),
    InstanceValue("KEY_W", Napi::Number::New(env, Robot::Key::KeyW)),
    InstanceValue("KEY_X", Napi::Number::New(env, Robot::Key::KeyX)),
    InstanceValue("KEY_Y", Napi::Number::New(env, Robot::Key::KeyY)),
    InstanceValue("KEY_Z", Napi::Number::New(env, Robot::Key::KeyZ)),

    InstanceValue("KEY_GRAVE", Napi::Number::New(env, Robot::Key::KeyGrave)),
    InstanceValue("KEY_MINUS", Napi::Number::New(env, Robot::Key::KeyMinus)),
    InstanceValue("KEY_EQUAL", Napi::Number::New(env, Robot::Key::KeyEqual)),
    InstanceValue("KEY_BACKSPACE", Napi::Number::New(env, Robot::Key::KeyBackspace)),
    InstanceValue("KEY_LBRACKET", Napi::Number::New(env, Robot::Key::KeyLBracket)),
    InstanceValue("KEY_RBRACKET", Napi::Number::New(env, Robot::Key::KeyRBracket)),
    InstanceValue("KEY_BACKSLASH", Napi::Number::New(env, Robot::Key::KeyBackslash)),
    InstanceValue("KEY_SEMICOLON", Napi::Number::New(env, Robot::Key::KeySemicolon)),
    InstanceValue("KEY_QUOTE", Napi::Number::New(env, Robot::Key::KeyQuote)),
    InstanceValue("KEY_RETURN", Napi::Number::New(env, Robot::Key::KeyReturn)),
    InstanceValue("KEY_COMMA", Napi::Number::New(env, Robot::Key::KeyComma)),
    InstanceValue("KEY_PERIOD", Napi::Number::New(env, Robot::Key::KeyPeriod)),
    InstanceValue("KEY_SLASH", Napi::Number::New(env, Robot::Key::KeySlash)),

    InstanceValue("KEY_LEFT", Napi::Number::New(env, Robot::Key::KeyLeft)),
    InstanceValue("KEY_UP", Napi::Number::New(env, Robot::Key::KeyUp)),
    InstanceValue("KEY_RIGHT", Napi::Number::New(env, Robot::Key::KeyRight)),
    InstanceValue("KEY_DOWN", Napi::Number::New(env, Robot::Key::KeyDown)),

    InstanceValue("KEY_PRINT", Napi::Number::New(env, Robot::Key::KeyPrint)),
    InstanceValue("KEY_PAUSE", Napi::Number::New(env, Robot::Key::KeyPause)),
    InstanceValue("KEY_INSERT", Napi::Number::New(env, Robot::Key::KeyInsert)),
    InstanceValue("KEY_DELETE", Napi::Number::New(env, Robot::Key::KeyDelete)),
    InstanceValue("KEY_HOME", Napi::Number::New(env, Robot::Key::KeyHome)),
    InstanceValue("KEY_END", Napi::Number::New(env, Robot::Key::KeyEnd)),
    InstanceValue("KEY_PAGE_UP", Napi::Number::New(env, Robot::Key::KeyPageUp)),
    InstanceValue("KEY_PAGE_DOWN", Napi::Number::New(env, Robot::Key::KeyPageDown)),

    InstanceValue("KEY_ADD", Napi::Number::New(env, Robot::Key::KeyAdd)),
    InstanceValue("KEY_SUBTRACT", Napi::Number::New(env, Robot::Key::KeySubtract)),
    InstanceValue("KEY_MULTIPLY", Napi::Number::New(env, Robot::Key::KeyMultiply)),
    InstanceValue("KEY_DIVIDE", Napi::Number::New(env, Robot::Key::KeyDivide)),
    InstanceValue("KEY_DECIMAL", Napi::Number::New(env, Robot::Key::KeyDecimal)),
    InstanceValue("KEY_ENTER", Napi::Number::New(env, Robot::Key::KeyEnter)),

    InstanceValue("KEY_NUM0", Napi::Number::New(env, Robot::Key::KeyNum0)),
    InstanceValue("KEY_NUM1", Napi::Number::New(env, Robot::Key::KeyNum1)),
    InstanceValue("KEY_NUM2", Napi::Number::New(env, Robot::Key::KeyNum2)),
    InstanceValue("KEY_NUM3", Napi::Number::New(env, Robot::Key::KeyNum3)),
    InstanceValue("KEY_NUM4", Napi::Number::New(env, Robot::Key::KeyNum4)),
    InstanceValue("KEY_NUM5", Napi::Number::New(env, Robot::Key::KeyNum5)),
    InstanceValue("KEY_NUM6", Napi::Number::New(env, Robot::Key::KeyNum6)),
    InstanceValue("KEY_NUM7", Napi::Number::New(env, Robot::Key::KeyNum7)),
    InstanceValue("KEY_NUM8", Napi::Number::New(env, Robot::Key::KeyNum8)),
    InstanceValue("KEY_NUM9", Napi::Number::New(env, Robot::Key::KeyNum9)),

    InstanceValue("KEY_CAPS_LOCK", Napi::Number::New(env, Robot::Key::KeyCapsLock)),
    InstanceValue("KEY_SCROLL_LOCK", Napi::Number::New(env, Robot::Key::KeyScrollLock)),
    InstanceValue("KEY_NUM_LOCK", Napi::Number::New(env, Robot::Key::KeyNumLock)),

    InstanceValue("BUTTON_LEFT", Napi::Number::New(env, Robot::Button::ButtonLeft)),
    InstanceValue("BUTTON_MID", Napi::Number::New(env, Robot::Button::ButtonMid)),
    InstanceValue("BUTTON_MIDDLE", Napi::Number::New(env, Robot::Button::ButtonMiddle)),
    InstanceValue("BUTTON_RIGHT", Napi::Number::New(env, Robot::Button::ButtonRight)),
    InstanceValue("BUTTON_X1", Napi::Number::New(env, Robot::Button::ButtonX1)),
    InstanceValue("BUTTON_X2", Napi::Number::New(env, Robot::Button::ButtonX2)),
  });
}

NODE_API_ADDON(RobotAdapter);
