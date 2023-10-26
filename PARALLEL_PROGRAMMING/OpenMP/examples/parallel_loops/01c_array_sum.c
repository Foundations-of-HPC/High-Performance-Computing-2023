
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

/*
 *  This code is equivalent to 01_array_sum.c but for the fact
 *  the it prints the value of the loop counter at the first iteration
 *  of every thread; in this way you can spy how the iteration space
 *  is subdivided with a simple #pragma omp for
 *
 */


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#if !defined(_OPENMP)
#error "OpenMP support needed for this code"
#endif
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>


#define N_default 100


int main( int argc, char **argv )
{

  int     N        = N_default;
  int     nthreads = 1;  
  double *array;

  /*  -----------------------------------------------------------------------------
   *   initialize 
   *  -----------------------------------------------------------------------------
   */


  // just give notice of what will happen and get the number of threads used
 #pragma omp parallel
 #pragma omp master
  nthreads = omp_get_num_threads();

  printf("omp summation with %d threads\n", nthreads );

  // allocate memory
  if ( (array = (double*)calloc( N, sizeof(double) )) == NULL )
    {
      printf("I'm sorry, there is not enough memory to host %lu bytes\n", N * sizeof(double) );
      return 1;
    }
  // initialize the array
  for ( int ii = 0; ii < N; ii++ )
    array[ii] = (double)ii;


  /*  -----------------------------------------------------------------------------
   *   calculate
   *  -----------------------------------------------------------------------------
   */


  double S           = 0;                                   // this will store the summation
  
 #pragma omp parallel 
  {
    int me      = omp_get_thread_num();
    int i, first = 1;
    
    printf("thread %d : &i is %p\n", me, &i);
   #pragma omp for reduction(+:S)                              
    for ( i = 0; i < N; i++ )
      {
	if ( first ) {
	  printf("\tthread %d : &loopcounter is %p\n", me, &i); first = 0;}
	S += array[i];
      }    
  }
  

  /*  -----------------------------------------------------------------------------
   *   finalize
   *  -----------------------------------------------------------------------------
   */

  printf("Sum is %g\n\n", S );
  
  free( array );
  
  return 0;
}
