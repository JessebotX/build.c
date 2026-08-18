#define BUILD_DISABLE_SHORT_NAMES
#define BUILD_IMPLEMENTATION
#include "build.h"

#include <stdio.h>

int main(int argc, char** argv)
{
#if 0
   {
      printf("== START Test 1 ==\n");

      StrList l = strlist_from_c(NULL);
      assert(l.bytes);
      assert(l.len == 0);
      assert(l.cap == 1);

      printf("=== END Test 1 ===\n\n");
   }

   {
      printf("== START Test 2 ==\n");

      const char* data[] = { "hello", "world", NULL };
      StrList l = strlist_from_c(data);
      assert(l.len == 2);
      assert(l.cap == 3);
      for (int i = 0; i < l.len; i++) {
         printf("%s\n", l.bytes[i]);
      }
      strlist_free(&l);

      printf("=== END Test 2 ===\n\n");
   }

   {
      printf("== START Test 3 ==\n");

      StrList l = strlist_from_args("file1.txt", "file2.txt", "file3.txt", NULL);
      for (int i = 0; i < l.len; i++) {
         printf("%s\n", l.bytes[i]);
      }
      strlist_free(&l);

      printf("=== END Test 3 ===\n\n");
   }

   {
      printf("== START Test 3 ==\n");

      StrList l = {0};
      strlist_append_args(&l, "test 1", "test 2", "test 3", "test 4", "test 5", NULL);

      for (int i = 0; i < l.len; i++) {
         printf("%s\n", l.bytes[i]);
      }
      strlist_free(&l);

      printf("=== END Test 3 ===\n\n");
   }

   {
      printf("== START Test 4 ==\n");

      const char* data[] = {
         "clang",
         "-DBUILD_IMPLEMENTATION=1",
         "-DBUILD_DISABLE_SHORT_NAMES=1",
         "-std=c89",
         "-pedantic",
         "-Wall",
         "-Wextra",
         "-Werror",
         "-Werror=implicit-function-declaration",
         "-Werror=c++-compat",
         "build.h",
         "-o",
         "build_test.exe",
         NULL
      };
      BUILD_ASSERT(process_execute_c(data));

      printf("=== END Test 4 ===\n\n");
   }
#endif
   Build_Artifact artifact = (Build_Artifact){
      .compiler = "clang",
      .output_dir = "out",
      .compile_options = build_strlist_from_args("-O0", "-g3", NULL),
      .source_files = build_strlist_from_args("build.c", NULL),
   };
   build_artifact_new_executable(&artifact, "my_program");

   return 0;
}
