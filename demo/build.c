#define BUILD_IMPLEMENTATION
#include "../build.h"

#include <stdio.h>

int main(int argc, char** argv)
{
   new_directory("testdata/hello/world");

   {
      CStringList l = { 0 };
      int i = 0;

      append_to_cstring_list(&l, "clang");
      append_to_cstring_list(&l, "-Wall");
      append_to_cstring_list(&l, "-Wextra");
      append_to_cstring_list(&l, "src/main.c");
      append_to_cstring_list(&l, "-o");
      append_to_cstring_list(&l, "test.exe");

      for (i = 0; i < l.count; i++) {
         printf("%s ", l.data[i]);
      }
      printf("\n");
   }

   return 0;
}
