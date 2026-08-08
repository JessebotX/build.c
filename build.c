#define BUILD_IMPLEMENTATION
#include "build.h"

#include <stdio.h>

int main(int argc, char** argv)
{
   StringList cmd = string_list_new(NULL);

   BUILD_ASSERT(string_list_append(&cmd, "clang"));
   BUILD_ASSERT(string_list_append(&cmd, "-DBUILD_IMPLEMENTATION=1"));
   BUILD_ASSERT(string_list_append(&cmd, "-std=c89"));
   BUILD_ASSERT(string_list_append(&cmd, "-pedantic"));
   BUILD_ASSERT(string_list_append(&cmd, "-Wall"));
   BUILD_ASSERT(string_list_append(&cmd, "-Wextra"));
   BUILD_ASSERT(string_list_append(&cmd, "-Werror"));
   BUILD_ASSERT(string_list_append(&cmd, "-Werror=implicit-function-declaration"));
   BUILD_ASSERT(string_list_append(&cmd, "-Wc++-compat"));
   BUILD_ASSERT(string_list_append(&cmd, "-Wno-unused-function"));
   BUILD_ASSERT(string_list_append(&cmd, "-Wno-unused-variable"));
   BUILD_ASSERT(string_list_append(&cmd, "-Wno-unused-parameter"));
   BUILD_ASSERT(string_list_append(&cmd, "-Wno-unused-but-set-variable"));
   BUILD_ASSERT(string_list_append(&cmd, "-c"));
   BUILD_ASSERT(string_list_append(&cmd, "build.h"));
   BUILD_ASSERT(string_list_append(&cmd, "-o"));
   BUILD_ASSERT(string_list_append(&cmd, "build.o"));

   for (int i = 0; i < cmd.count; i++) {
      printf("%s\n", cmd.data[i]);
   }

   printf("count: %d\n", cmd.count);
   printf("capacity: %d\n", cmd.capacity);

   string_list_free_all(&cmd);

   return 0;
}
