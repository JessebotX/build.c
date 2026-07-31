#define BUILD_IMPLEMENTATION
#include "../build.h"

#include <stdio.h>

int main(int argc, char** argv)
{
   new_directory("testdata/hello/world");

   {
      StringList l = { 0 };
      StringBuffer s = { 0 };
      int i = 0;

      append_to_string_list(&l, "clang");
      append_to_string_list(&l, "-Wall");
      append_to_string_list(&l, "-Wextra");
      append_to_string_list(&l, "demo/main.c");
      append_to_string_list(&l, "-o");
      append_to_string_list(&l, "demo.exe");

      win32_join_and_quote_command_list(&l, &s);
      printf("%s\n", s.data);

      free_string_buffer(&s);
      free_string_list(&l);
   }

   return 0;
}
