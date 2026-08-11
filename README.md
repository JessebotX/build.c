# build.c/h

> NOTE: WIP: only has basic functionality such as creating directories and running commands on Windows systems only.

A C89+ [stb-style](https://github.com/nothings/stb) header-only single-file C library for writing build recipes in C. Inspired by the likes of [`nob.h`](https://github.com/tsoding/nob.h) and `build.zig` from the [Zig programming language](https://ziglang.org/).

The goal is to be able to manage a project written in C by only using C and a C89+/C++ compiler. That means no external shell script or build system tool required, just do the following (on Windows `cmd.exe` using `clang` compiler):

```sh
clang build.c -o build.exe
build
```

where `build.c` is in the same directory as `build.h` and has the following contents:

```c
/// build.c

// NOTE: (1) WIP, API subject to change
//       (2) File is written using C99+ features, but can be rewritten
//           to support C89

#define BUILD_IMPLEMENTATION
#include "build.h"

int main(int argc, char** argv)
{
   CompileTarget target = {
      .compiler = "clang",
      .compile_flags = string_list_new_varargs(
         "-Wall",
         "-Wextra",
         "-O0",
         "-g3",
         NULL
      ),
      .source_files = string_list_new_varargs("main.c", "base.c", NULL),
   };

   target_compile_executable(&target, "my_exe_name"); // on Windows, .exe is automatically appended onto the name

   return 0;
}
```

# License

SPDX-License-Identifier: `0BSD`

See [LICENSE.txt](LICENSE.txt).
