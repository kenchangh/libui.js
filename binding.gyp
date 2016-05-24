{
  "targets": [
    {
      "target_name": "liblibui",
      "sources": ["libui_js.cc", "window.cc", "utils.cc"],
      "libraries": ["-Wl,-rpath, ../libui/out/libui.so.0"],
    }
  ]
}