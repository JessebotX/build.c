# build.h

> NOTE: currently only supports Windows systems.

A C89+ [stb-style](https://github.com/nothings/stb) single-file header-only C library for writing build recipes in C. Inspired by the likes of [`nob.h`](https://github.com/tsoding/nob.h) and Zig's build system [`build.zig`](https://ziglang.org/learn/build-system/).

The goal is to be able to manage a project written in C by only using C and a C89+ or a C++ compiler. That means no external shell script or build system tool required, just run the following commands in a supported project (e.g. on Windows `cmd.exe` using `clang` compiler):

```sh
clang build.c -o build.exe
build
```

where `build.c` is in the same directory as `build.h` and has the following contents:

```c
/// build.c

// Last updated: 2026-08-16

// NOTE: (1) WIP, API subject to change
//       (2) File is written using C99+ features, but can be rewritten
//           to support C89

#define BUILD_IMPLEMENTATION
#include "build.h"

int main(int argc, char** argv)
{
   Artifact artifact = (Artifact){
      .compiler = "clang",
      .compile_options = strlist_from_args("-O0", "-g3", NULL),
      .source_files = strlist_from_args("main.c", NULL),
   };
   artifact_new_executable(&artifact, "my_exe_name");

   return 0;
}
```

# License

SPDX-License-Identifier: `0BSD`

See [LICENSE.txt](LICENSE.txt).
