{
  "variables": {
    "module_name%": "robot",
  },
  "target_defaults": {
    "conditions": [
      ["OS=='linux'", {
        "libraries": [
          "-lrt",
          "-lX11",
          "-lXtst",
          "-lXinerama",
        ],
      }],
      ["OS=='win'", {
        "libraries": [
          "-lPsapi",
        ],
      }],
      ["OS=='mac'", {
        "libraries": [
          "-framework ApplicationServices",
          "-framework AppKit",
        ],
      }],
    ],
    "configurations": {
      "Debug": {
        "defines": [
          "DEBUG",
        ],
      },
      "Release": {
        "defines": [
          "NDEBUG",
        ],
      },
    },
  },
  "targets": [
    {
      "target_name": "librobot",
      "type": "static_library",
      "configurations": {
        "Debug": {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1,
            },
          },
        },
        "Release": {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1,
            },
          },
        },
      },
      "xcode_settings": {
        "MACOSX_DEPLOYMENT_TARGET": "10.7",
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "GCC_VERSION": "com.apple.compilers.llvm.clang.1_0",
        "CLANG_CXX_LIBRARY": "libc++",
        "CLANG_CXX_LANGUAGE_STANDARD": "c++11",
        "OTHER_CPLUSPLUSFLAGS": [
          "-x objective-c++",
          "-Wno-sign-compare",
          "-Wno-unused-variable",
          "-Wno-missing-field-initializers",
        ],
      },
      "cflags_cc!": [
        "-fno-exceptions",
      ],
      "cflags_cc": [
        "-std=c++11",
        "-fexceptions",
        "-Wno-strict-aliasing",
        "-Wno-unused-variable",
        "-Wno-missing-field-initializers",
      ],
      "defines!": [
        "_HAS_EXCEPTIONS=0",
        "V8_DEPRECATION_WARNINGS=1",
      ],
      "sources": [
        "src/Robot/Bounds.cc",
        "src/Robot/Clipboard.cc",
        "src/Robot/Color.cc",
        "src/Robot/Hash.cc",
        "src/Robot/Image.cc",
        "src/Robot/Keyboard.cc",
        "src/Robot/Memory.cc",
        "src/Robot/Module.cc",
        "src/Robot/Mouse.cc",
        "src/Robot/Point.cc",
        "src/Robot/Process.cc",
        "src/Robot/Range.cc",
        "src/Robot/Screen.cc",
        "src/Robot/Size.cc",
        "src/Robot/Timer.cc",
        "src/Robot/Window.cc",
      ],
    },
    {
      "target_name": "<(module_name)",
      "dependencies": ["librobot"],
      "configurations": {
        "Debug": {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1,
            },
          },
        },
        "Release": {
          "msvs_settings": {
            "VCCLCompilerTool": {
              "ExceptionHandling": 1,
            },
          },
        },
      },
      "xcode_settings": {
        "MACOSX_DEPLOYMENT_TARGET": "10.7",
        "GCC_ENABLE_CPP_EXCEPTIONS": "YES",
        "GCC_VERSION": "com.apple.compilers.llvm.clang.1_0",
        "CLANG_CXX_LIBRARY": "libc++",
        "CLANG_CXX_LANGUAGE_STANDARD": "c++11",
        "OTHER_CPLUSPLUSFLAGS": [
          "-x objective-c++",
          "-Wno-sign-compare",
          "-Wno-unused-variable",
          "-Wno-missing-field-initializers",
        ],
      },
      "cflags_cc!": [
        "-fno-exceptions",
      ],
      "cflags_cc": [
        "-std=c++11",
        "-fexceptions",
        "-Wno-strict-aliasing",
        "-Wno-unused-variable",
        "-Wno-missing-field-initializers",
      ],
      "defines!": [
        "_HAS_EXCEPTIONS=0",
        "V8_DEPRECATION_WARNINGS=1",
      ],
      "include_dirs": [
        "src/Robot",
      ],
      "sources": [
        "src/Addon/Clipboard.cc",
        "src/Addon/Image.cc",
        "src/Addon/Keyboard.cc",
        "src/Addon/Memory.cc",
        "src/Addon/Mouse.cc",
        "src/Addon/Process.cc",
        "src/Addon/Robot.cc",
        "src/Addon/Screen.cc",
        "src/Addon/Window.cc",
      ],
    },
    {
      "target_name": "copy_binary",
      "type": "none",
      "dependencies": ["<(module_name)"],
      "copies": [
        {
          "files": ["<(PRODUCT_DIR)/<(module_name).node"],
          "destination": "<(module_path)",
        },
      ],
    },
  ],
}
