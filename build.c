#define BUILD_IMPLEMENTATION
#include "build.h"

int main(int argc, char** argv)
{
   CompileTarget target = {
      .compiler = "clang",
      .compile_flags = string_list_new_varargs(
         "-std=c89",
         "-pedantic",
         "-Wall",
         "-Wextra",
         "-Werror",
         "-Werror=implicit-function-declaration",
         "-Wc++-compat",
         "-DBUILD_IMPLEMENTATION=1",
         NULL
      ),
      .source_files = string_list_new_varargs("build.h", NULL),
   };

   target_compile_object(&target, "build");

   return 0;
}
