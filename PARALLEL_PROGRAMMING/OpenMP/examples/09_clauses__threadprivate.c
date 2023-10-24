
/* ────────────────────────────────────────────────────────────────────────── *
 │                                                                            │
 │ This file is part of the exercises for the Lectures on                     │
 │   "Foundations of High Performance Computing"                              │
 │ given at                                                                   │
 │   Master in HPC and                                                        │
 │   Master in Data Science and Scientific Computing                          │
 │ @ SISSA, ICTP and University of Trieste                                    │
 │                                                                            │
 │ contact: luca.tornatore@inaf.it                                            │
 │                                                                            │
 │     This is free software; you can redistribute it and/or modify           │
 │     it under the terms of the GNU General Public License as published by   │
 │     the Free Software Foundation; either version 3 of the License, or      │
 │     (at your option) any later version.                                    │
 │     This code is distributed in the hope that it will be useful,           │
 │     but WITHOUT ANY WARRANTY; without even the implied warranty of         │
 │     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the          │
 │     GNU General Public License for more details.                           │
 │                                                                            │
 │     You should have received a copy of the GNU General Public License      │
 │     along with this program.  If not, see <http://www.gnu.org/licenses/>   │
 │                                                                            │
 * ────────────────────────────────────────────────────────────────────────── */


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>


int  me, myN;
int *array;

#pragma omp threadprivate( me, myN, array )


#define DEFAULT 100000

int main( int argc, char **argv )
{
  int    N = ( argc > 1 ? atoi(*(argv+1)) : DEFAULT);

 #pragma omp parallel 
  {
    me = omp_get_thread_num();
    
    int nthreads = omp_get_num_threads();

    // note that we did not declare neither
    // myN nor array nor me in this scope
    //
    myN = (N / nthreads) + (me < N%nthreads);
    array = (int*)calloc( myN, sizeof(int) );

    printf("+ thread %d has got %d elements; local array "
	   "(address stored in %p) starts at %p\n",
	   me, myN, &array, array );
    
    // write something in the array
    //

    int max = ( myN > 3 ? 3 : myN );
    for( int j = 0; j < max; j++ )
      array[j] = me*1000 + j;
  }


  printf("\nnow we are again in a serial region\n\n");
  

 #pragma omp parallel 
  {
    char buffer[200];
    sprintf( buffer, "* thread %d :: ", me );
    
    int max = ( myN > 3 ? 3 : myN );
    for( int j = 0; j < max; j++ )
      sprintf( &buffer[strlen(buffer)], "[%d] = %4d , ", j, array[j] );

    printf("%s\n", buffer );

    // we must free array from within a parallel region
    // is we did this in a serial region, only the memory
    // associated to the master thread would be freed
    //
    free(array);
  }


  return 0;
}

