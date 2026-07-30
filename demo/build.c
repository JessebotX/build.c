#define BUILD_IMPLEMENTATION
#include "../build.h"

#include <stdio.h>

int main(int argc, char** argv)
{
   printf("Hello, world!\n");

   new_directory("testdata/hello/world");

   return 0;
}
