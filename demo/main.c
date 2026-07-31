#include <stdio.h>

#define hello_world printf

int main(void)
{
   printf("hello_world\n");
   hello_world("printf\n");

   return 0;
}
