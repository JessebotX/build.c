#define BUILD_IMPLEMENTATION
#include "../build.h"

#include <stdio.h>

int main(int argc, char** argv)
{
   printf("Hello, world!\n");

   directory_new("testdata/hello/world");

   return 0;
}
