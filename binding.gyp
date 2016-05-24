{
  "targets": [
    {
      "target_name": "liblibui",
      "sources": ["libui_js.cc", "libui_methods.cc", "window.cc", "utils.cc"],
      "libraries": ["-Wl,-rpath, ../libui/out/libui.so.0"],
    }
  ]
}