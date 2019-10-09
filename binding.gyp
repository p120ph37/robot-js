{
	"conditions":
	[
		[ "OS == 'linux'", { "variables": { "target_platform": "linux"  }}],
		[ "OS == 'mac'",   { "variables": { "target_platform": "darwin" }}],
		[ "OS == 'win'",   { "variables": { "target_platform": "win32"  }}]
	],

	"targets":
	[
		{
			"target_name": "<(module_name)",

			"sources":
			[
				"src/robot/Source/Hash.cc",
				"src/robot/Source/Color.cc",
				"src/robot/Source/Image.cc",
				"src/robot/Source/Range.cc",
				"src/robot/Source/Point.cc",
				"src/robot/Source/Size.cc",
				"src/robot/Source/Bounds.cc",

				"src/robot/Source/Keyboard.cc",
				"src/robot/Source/Mouse.cc",
				"src/robot/Source/Process.cc",
				"src/robot/Source/Module.cc",
				"src/robot/Source/Memory.cc",
				"src/robot/Source/Window.cc",
				"src/robot/Source/Screen.cc",
				"src/robot/Source/Timer.cc",
				"src/robot/Source/Clipboard.cc",

				"src/NodeRobot.cc",
				"src/NodeImage.cc",

				"src/NodeKeyboard.cc",
				"src/NodeMouse.cc",
				"src/NodeProcess.cc",
				"src/NodeMemory.cc",
				"src/NodeWindow.cc",
				"src/NodeScreen.cc",
				"src/NodeClipboard.cc"
			],

			"include_dirs": [ "src/robot/Source/", "<!(node -e \"require('nan')\")" ],

			"conditions":
			[
				[ "OS == 'linux'", {

					"libraries":
					[
						"-lrt",
						"-lX11",
						"-lXtst",
						"-lXinerama"
					],

					"defines!":
					[
						"V8_DEPRECATION_WARNINGS=1"
					],

					"cflags_cc":
					[
						"-std=c++11",
						"-fexceptions",
						"-Wno-strict-aliasing",
						"-Wno-unused-variable",
						"-Wno-missing-field-initializers"
					],

					"cflags_cc!":
					[
						"-std=gnu++0x",
						"-fno-exceptions"
					]
				}],

				[ "OS == 'mac'", {

					"libraries":
					[
						"-framework ApplicationServices",
						"-framework AppKit"
					],

					"defines!":
					[
						"V8_DEPRECATION_WARNINGS=1"
					],

					"xcode_settings":
					{
						"MACOSX_DEPLOYMENT_TARGET": "10.8",
						"CLANG_CXX_LANGUAGE_STANDARD": "c++11",
						"CLANG_CXX_LIBRARY": "libc++",
						"GCC_ENABLE_CPP_EXCEPTIONS": "YES",
						"GCC_VERSION": "com.apple.compilers.llvm.clang.1_0",

						"OTHER_CPLUSPLUSFLAGS":
						[
							"-x objective-c++",
							"-Wno-sign-compare",
							"-Wno-unused-variable",
							"-Wno-missing-field-initializers"
						]
					}
				}],

				[ "OS == 'win'", {

					"libraries":
					[
						"-lPsapi"
					],

					"defines!":
					[
						"_HAS_EXCEPTIONS=0",
						"V8_DEPRECATION_WARNINGS=1"
					],

					"sources":
					[
						"src/NodeRobot.rc"
					],

					"configurations":
					{
						"Debug":
						{
							"defines":
							[
								"UNICODE",
								"DEBUG"
							],

							"msvs_settings":
							{
								"VCCLCompilerTool": { "ExceptionHandling": 1 },
								"VCLinkerTool":     { "SubSystem":         2 }
							},

							"msvs_disabled_warnings": [ 4005, 4661, 4011 ]

						},

						"Release":
						{
							"defines":
							[
								"UNICODE",
								"NDEBUG"
							],

							"msvs_settings":
							{
								"VCCLCompilerTool": { "ExceptionHandling": 1 },
								"VCLinkerTool":     { "SubSystem":         2 }
							},

							"msvs_disabled_warnings": [ 4005, 4661, 4011 ]
						}
					},

				}]
			]
		},
		{
			"target_name": "copy_binary",
			"type": "none",

			"dependencies":
			[
				"<(module_name)"
			],

			"copies":
			[
				{
					"conditions":
					[
						[ "OS == 'linux'", {

							"files":
							[
								"<(PRODUCT_DIR)/<@(_dependencies).node"
							],

							"destination": "<(module_path)"
						}],

						[ "OS == 'mac'", {

							"files":
							[
								"<(PRODUCT_DIR)/<@(_dependencies).node"
							],

							"destination": "<(module_path)"
						}],

						[ "OS == 'win'", {

							"files":
							[
								"<(PRODUCT_DIR)/<@(_dependencies).node",
								"<(PRODUCT_DIR)/<@(_dependencies).pdb"
							],

							"destination": "<(module_path)"
						}]
					]
				}
			]
		}
	]
}
