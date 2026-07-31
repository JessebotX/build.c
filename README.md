# build.c/h

> NOTE: WIP: only has basic functionality such as creating directories and running commands on Windows systems only.

A C89+ [stb-style](https://github.com/nothings/stb) header-only single-file C library for writing build recipes in C. Inspired by the likes of [`nob.h`](https://github.com/tsoding/nob.h) and `build.zig` from the [Zig programming language](https://ziglang.org/).

The goal is to be able to manage a project written in C by only using C and a C89+/C++ compiler. That means no external shell script or build system tool required, just do the following:

```sh
clang build.c -o build.exe # Similar to running `cmake`, can use any other C89+
                           # compiler such as gcc, microsoft's cl, tcc, or a C++ 
                           # compiler.

./build.exe                  # Similar to running `cmake --build`
```

where `build.c` is in the same directory as `build.h` and has the following contents (WIP, API will be changed):

```c
// build.c //
#define BUILD_IMPLEMENTATION
#include "build.h"

int main(int argc, char** argv)
{
  StringList l = { 0 };

  append_to_string_list(&l, "clang");
  append_to_string_list(&l, "-Wall");
  append_to_string_list(&l, "-Wextra");
  append_to_string_list(&l, "/path/to/your/c/source/file.c");
  append_to_string_list(&l, "-o");
  append_to_string_list(&l, "/path/to/your/c/output/executable.exe");

  win32_run_command(&l);

  free_string_list(&l);

  return 0;
}
```

# License

0BSD
