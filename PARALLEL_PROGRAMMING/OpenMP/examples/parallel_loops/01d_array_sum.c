
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
 *  This code is equivalent to 01_array_sum.c but for the fact that
 *  the reduction is implemented by hands instead of using the reduction()
 *  clause
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
#include <time.h>
#include <omp.h>



#if defined(_OPENMP)
#define CPU_TIME ({struct  timespec ts; clock_gettime( CLOCK_REALTIME, &ts ),\
					  (double)ts.tv_sec +		\
					  (double)ts.tv_nsec * 1e-9;})

#define CPU_TIME_th ({struct  timespec myts; clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ),\
					     (double)myts.tv_sec +	\
					     (double)myts.tv_nsec * 1e-9;})
#else

#define CPU_TIME ({struct  timespec ts; clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ),\
					  (double)ts.tv_sec +		\
					  (double)ts.tv_nsec * 1e-9;})

#endif

#define N_default 100


int main( int argc, char **argv )
{

  unsigned long long int N = N_default;
  int     nthreads = 1;  
  double *array;

  /*  -----------------------------------------------------------------------------
   *   initialize 
   *  -----------------------------------------------------------------------------
   */

  // check whether some arg has been passed on
  if ( argc > 1 )
    N = (unsigned long long int)atoll( *(argv+1) );


  // just give notice of what will happen and get the number of threads used
 #pragma omp parallel
 #pragma omp master
  nthreads = omp_get_num_threads();

  printf("omp summation with %d threads\n", nthreads );

  // allocate memory
  if ( (array = (double*)calloc( N, sizeof(double) )) == NULL )
    {
      printf("I'm sorry, there is not enough memory to host %llu bytes\n",
	     N * sizeof(double) );
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
  double tstart      = CPU_TIME;
  
 #pragma omp parallel
  {
    double myS = 0;
   #pragma omp for
    for (int ii = 0; ii < N; ii++ )
      myS += array[ii];
    
   #pragma omp atomic update
    S += myS;
  }
  
  
  /*  -----------------------------------------------------------------------------
   *   finalize
   *  -----------------------------------------------------------------------------
   */
  
  double tend = CPU_TIME;
  printf("Sum is %g, process took %g of wall-clock time\n\n",
	 S, tend - tstart );
  
  free( array );
  
  return 0;
}
