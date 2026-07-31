#include <stdio.h>

#define hello_world printf

int main(int argc, char** argv)
{
   printf("hello_world\n");
   hello_world("printf\n");

   return 0;
}
