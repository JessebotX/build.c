#define BUILD_IMPLEMENTATION
#include "build.h"

int main(int argc, char** argv)
{
   StringList cmd = string_list_new_capacity(NULL, 20);

#if 0
   string_list_append(&cmd, "-O0");
   string_list_append(&cmd, "-g3");
   string_list_append(&cmd, "demo/main.c");
   string_list_append(&cmd, "-o");
   string_list_append(&cmd, "out/demo.exe");

   command_execute(&cmd);
#endif

   return 0;
}
