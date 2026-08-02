#define BUILD_IMPLEMENTATION
#include "build.h"

int main(int argc, char** argv)
{
   StringList cmd = { 0 };

   build_new_directory("out");

   append_to_string_list(&cmd, "clang");
   append_to_string_list(&cmd, "-O0");
   append_to_string_list(&cmd, "-g3");
   append_to_string_list(&cmd, "demo/main.c");
   append_to_string_list(&cmd, "-o");
   append_to_string_list(&cmd, "out/demo.exe");

   run_command(&cmd);

   return 0;
}
