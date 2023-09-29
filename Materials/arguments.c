
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int main (int argc, char **argv )
{

  printf("argv is located at address %p and points to %p\n", &argv, argv );
  
  int i = 0;
  while ( i < argc )
    {
      printf("arguments %d is located at address %p and reads as %s\n", i, argv + i, *(argv+i));
      i++;
    }

  return 0;
}
