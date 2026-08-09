#define BUILD_IMPLEMENTATION
#include "build.h"

#include <stdio.h>

int main(int argc, char** argv)
{
   StringList cmd_args = string_list_new(NULL);
   BUILD_ASSERT(cmd_args.capacity > 0);

   {
      BUILD_ASSERT(string_list_append_string(&cmd_args, "clang"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-DBUILD_IMPLEMENTATION=1"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-std=c89"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-pedantic"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wall"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wextra"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Werror"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Werror=implicit-function-declaration"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wc++-compat"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wno-unused-function"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wno-unused-variable"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wno-unused-parameter"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wno-unused-but-set-variable"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-c"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "build.h"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-o"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "build.clang.o"));

      // process_execute(&cmd_args);

      string_list_free_items(&cmd_args);
   }

   {
      BUILD_ASSERT(string_list_append_string(&cmd_args, "clang++"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-DBUILD_IMPLEMENTATION=1"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-std=c++11"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-pedantic"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wall"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wextra"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Werror"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Werror=implicit-function-declaration"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wc++-compat"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wno-deprecated"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wno-unused-function"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wno-unused-variable"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wno-unused-parameter"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-Wno-unused-but-set-variable"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-c"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "build.h"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-o"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "build.clang.cpp.o"));

      // process_execute(&cmd_args);

      string_list_free_items(&cmd_args);
   }

   {
      BUILD_ASSERT(string_list_append_string(&cmd_args, "tcc"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "build.c"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "-o"));
      BUILD_ASSERT(string_list_append_string(&cmd_args, "build-tcc-test.exe"));

      // process_execute(&cmd_args);

      string_list_free_items(&cmd_args);
   }
   string_list_free_all(&cmd_args);

   {
      // const char* s[] = { "world", "!", "goodbye", "world", "!", NULL };
      StringList list1 = string_list_new(NULL);
      StringList list2 = string_list_new(NULL);

      BUILD_ASSERT(string_list_append_string(&list1, "hello"));
      BUILD_ASSERT(string_list_append_string(&list2, "world"));
      BUILD_ASSERT(string_list_append_string(&list2, "!"));
      BUILD_ASSERT(string_list_append_string(&list2, "goodbye"));
      BUILD_ASSERT(string_list_append_string(&list2, "world"));
      BUILD_ASSERT(string_list_append_string(&list2, "!"));

      printf("list1 count: %d\n", list1.count);
      printf("list1 capacity: %d\n", list1.capacity);
      printf("list2 count: %d\n", list2.count);
      printf("list2 capacity: %d\n", list2.capacity);

      BUILD_ASSERT(string_list_append_list(&list1, &list2));
      string_list_free_all(&list2);

      for (int i = 0; i < list1.count; i++) {
         printf("list1.data[%d] = %s\n", i, list1.data[i]);
      }

      printf("list1 count: %d\n", list1.count);
      printf("list1 capacity: %d\n", list1.capacity);

      string_list_free_all(&list1);
   }

   return 0;
}
