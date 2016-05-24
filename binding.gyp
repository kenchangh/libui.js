{
  "targets": [
    {
      "target_name": "liblibui",
      "include": ["lib"],
      "sources": ["libui_js.cc", "libui_methods.cc", "window.cc", "utils.cc"],
      "libraries": ["-Wl,-rpath, ../libui/out/libui.so.0"],
    }
  ]
}