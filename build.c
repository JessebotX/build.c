#define BUILD_IMPLEMENTATION
#include "build.h"

#include <stdio.h>

int main(int argc, char** argv)
{
   StringList cmd_args = string_list_new(NULL);
   BUILD_ASSERT(cmd_args.capacity > 0);

   {
      BUILD_ASSERT(string_list_append(&cmd_args, "clang"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-DBUILD_IMPLEMENTATION=1"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-std=c89"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-pedantic"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wall"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wextra"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Werror"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Werror=implicit-function-declaration"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wc++-compat"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wno-unused-function"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wno-unused-variable"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wno-unused-parameter"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wno-unused-but-set-variable"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-c"));
      BUILD_ASSERT(string_list_append(&cmd_args, "build.h"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-o"));
      BUILD_ASSERT(string_list_append(&cmd_args, "build.clang.o"));

      process_execute(&cmd_args);

      string_list_free_items(&cmd_args);
   }

   {
      BUILD_ASSERT(string_list_append(&cmd_args, "clang++"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-DBUILD_IMPLEMENTATION=1"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-std=c++11"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-pedantic"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wall"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wextra"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Werror"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Werror=implicit-function-declaration"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wc++-compat"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wno-deprecated"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wno-unused-function"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wno-unused-variable"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wno-unused-parameter"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-Wno-unused-but-set-variable"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-c"));
      BUILD_ASSERT(string_list_append(&cmd_args, "build.h"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-o"));
      BUILD_ASSERT(string_list_append(&cmd_args, "build.clang.cpp.o"));

      process_execute(&cmd_args);

      string_list_free_items(&cmd_args);
   }

   {

      BUILD_ASSERT(string_list_append(&cmd_args, "tcc"));
      BUILD_ASSERT(string_list_append(&cmd_args, "build.c"));
      BUILD_ASSERT(string_list_append(&cmd_args, "-o"));
      BUILD_ASSERT(string_list_append(&cmd_args, "build-tcc-test.exe"));

      process_execute(&cmd_args);

      string_list_free_items(&cmd_args);
   }

   printf("== END ==\n", cmd_args.count);
   printf("cmd_args.count = %d\n", cmd_args.count);
   printf("cmd_args.capacity = %d\n", cmd_args.capacity);

   string_list_free_all(&cmd_args);

   return 0;
}
