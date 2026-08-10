#define BUILD_DEF static
#define BUILD_IMPLEMENTATION
#include "build.h"

#include <stdio.h>

int main(int argc, char** argv)
{

   CompileTarget target = {
      .compiler = "clang",
      .compile_flags = string_list_new_variable(
         "-Wall",
         "-Wextra",
         "-Wno-unused-function",
         "-Wno-unused-parameter",
         "-Wno-unused-variable",
         "-Wno-unused-but-set-variable",
         NULL
      ),
      .linker_flags = string_list_new(NULL),
      .source_files = string_list_new_variable("build.c", NULL),
   };

   BUILD_ASSERT(target_compile_executable(&target, "build-target-test"));

   return 0;
}
