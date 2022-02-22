{
  "configurations": {
    "Debug": { "defines": ["DEBUG"] },
    "Release": { "defines": ["NDEBUG"] },
  },
  "defines": [
    "NAPI_VERSION=4",
  ],
  "targets": [
    {
      "target_name": "robot",
      "sources": [
        "src/robot/Source/Bounds.cc",
        "src/robot/Source/Clipboard.cc",
        "src/robot/Source/Color.cc",
        "src/robot/Source/Hash.cc",
        "src/robot/Source/Image.cc",
        "src/robot/Source/Keyboard.cc",
        "src/robot/Source/Memory.cc",
        "src/robot/Source/Module.cc",
        "src/robot/Source/Mouse.cc",
        "src/robot/Source/Point.cc",
        "src/robot/Source/Process.cc",
        "src/robot/Source/Range.cc",
        "src/robot/Source/Screen.cc",
        "src/robot/Source/Size.cc",
        "src/robot/Source/Timer.cc",
        "src/robot/Source/Window.cc",

        "src/BoundsAdapter.cc",
        "src/ClipboardAdapter.cc",
        "src/ColorAdapter.cc",
        "src/HashAdapter.cc",
        "src/ImageAdapter.cc",
        "src/KeyboardAdapter.cc",
        "src/MemoryAdapter.cc",
        "src/ModuleAdapter.cc",
        "src/MouseAdapter.cc",
        "src/PointAdapter.cc",
        "src/ProcessAdapter.cc",
        "src/RangeAdapter.cc",
        "src/ScreenAdapter.cc",
        "src/SizeAdapter.cc",
        "src/TimerAdapter.cc",
        "src/WindowAdapter.cc",

        "src/RobotAdapter.cc",
      ],
      "include_dirs": [
        "src/",
        "src/robot/Source/",
        "<!@(node -p \"require('node-addon-api').include\")",
      ],
      "cflags!": ["-fno-exceptions"],
      "cflags_cc!": ["-fno-exceptions"],
      "conditions": [
        [ "OS == 'linux'", {
          "libraries": ["-lrt", "-lX11", "-lXtst", "-lXinerama", "-static-libstdc++"],
          "cflags+": ["-Wno-missing-field-initializers", "-Wimplicit-fallthrough=0"],
        }],
        [ "OS == 'mac'", {
          "libraries": ["-framework ApplicationServices", "-framework AppKit"],
          "cflags+": ["-fvisibility=hidden"],
          "xcode_settings": {
            "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
            "CLANG_CXX_LIBRARY": "libc++",
            "MACOSX_DEPLOYMENT_TARGET": "10.7",
            "GCC_SYMBOLS_PRIVATE_EXTERN": "YES", # -fvisibility=hidden
            "OTHER_CPLUSPLUSFLAGS": [
              "-ObjC++", # even though this project itself isn't Obj-C, the frameworks are.
              "-Wno-sign-compare",
              "-Wno-missing-field-initializers",
            ],
          },
        }],
        [ "OS == 'win'", {
          "libraries": ["-lPsapi"],
          "defines!": ["_HAS_EXCEPTIONS=0"],
          "defines": ["UNICODE", "_HAS_EXCEPTIONS=1"],
          "msvs_settings": {
            "VCCLCompilerTool": {"ExceptionHandling": 1},
            "VCLinkerTool": {"SubSystem": 2}
          },
          "msvs_disabled_warnings": [4005, 4661],
        }],
      ],
    },
  ],
}
