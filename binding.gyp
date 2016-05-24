{
  "targets": [
    {
        "target_name": "liblibui",
        "include_dirs": ["lib"],
        "sources": [
            "libui_js.cc",
            "lib/window.cc",
            "lib/menu.cc",
            "lib/libui_methods.cc",
            "lib/utils.cc"
        ],
        "libraries": ["-Wl,-rpath, ../libui/out/libui.so.0"]
    }
  ]
}