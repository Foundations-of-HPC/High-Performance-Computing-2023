#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main( void )
{

  unsigned int i    = 128;
  int          size = sizeof(i);

  /*
   *  i is and integer variable, and as suche it requires 4 bytes
   *  let's explore how this 4 bytes are placed in memory
   */  
  
  for ( int j = 0; j < size; j++ )
    // 
    // we loop over the bytes that make up the variable i
    // note: to be general, we asked size to be the value
    //       returned by sizeof()
    //
    {
      // let's print the value of the entire bitfield
      // when we interpret it as an integer
      printf("i is: %d\n", i );

      // now we access each byte of i
      //
      char *byte = (char*)&i;
      for( int k = 0; k < size; k++ )
	printf("\t%p : %d\n", byte+k, *(byte+k) );

      // convince yourself that the previous for loop could have been
      // written as follows:
      // ( un-comment the next 2 lines to test it
      
      /* for( int k = 0; k < size; k++ ) */
      /* 	printf("\t%p : %d\n", (char*)&i+k, *(((char*)&i)+k)); */
      
      // why is it so ?
      // -- &i is the address of i; more precisely
      // it is the address of the begin of i, i.e.
      // the address of the furst of the bytes that
      // form i.
      // -- (char*)&i means that we interpret the
      // address &i as an address to a char
      // -- *(char*)&i reads as "the value of the byte
      // at the address &i"
      // -- (char*)&i+k is k-byte after the byte at
      // address &i
      

      printf("\n");

      // now we multiply i by 256.
      // the operators << and >> read as "shift the argument's bit on the left [or right]
      // by the specified amount of bits "
      // In this case the amount of bits is 8, i.e. is is the same than multiplying by 256
      //
      i <<= 8;

      // we are doing this because we want that only a single bit is set per each byte
      // among the i's bytes.
      // we started from a value of 1, i.e. only the first bit of the first byte of i was
      // set; multiplying  by 256 (i.e. bit-shifting by 8 positions) we move that bit
      // to he next byte.
    }

  return 0;
}
