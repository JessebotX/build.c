#define BUILD_IMPLEMENTATION
#include "../build.h"

#include <stdio.h>

int main(int argc, char** argv)
{
   {
      StringList l = { 0 };
      int i = 0;

      append_to_string_list(&l, "clang");
      append_to_string_list(&l, "-Wall");
      append_to_string_list(&l, "-Wextra");
      append_to_string_list(&l, "demo/main.c");
      append_to_string_list(&l, "-o");
      append_to_string_list(&l, "out/demo.exe");

      win32_run_command(&l);

      free_string_list(&l);
   }

   return 0;
}
