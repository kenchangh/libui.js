{
  "targets": [
    {
      "target_name": "liblibui",
      "sources": ["interface.cc"],
      "include_dirs": ["<!(node -e \"require('nan')\")"],
      "libraries": ["-Wl,-rpath, ../libui/out/libui.so.0"]
    }
  ]
}