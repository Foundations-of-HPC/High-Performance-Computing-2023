
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
 *  This code is equivalent to 01_array_sum.c but for this facts:
 *  
 *  (1) you can compile a serial version, without -fopenmp at command line
 *  
 *  (2) you can measure the execution time for all the threads
 *
 */


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


#define N_default 100

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





int main( int argc, char **argv )
{

  int     N        = N_default;
  int     nthreads = 1;
  
  double *array;



  /*  -----------------------------------------------------------------------------
   *   initialize 
   *  -----------------------------------------------------------------------------
   */

  // check whether some arg has been passed on
  if ( argc > 1 )
    N = atoi( *(argv+1) );


  // allocate memory
  if ( (array = (double*)malloc( N * sizeof(double) )) == NULL )
    {
      printf("I'm sorry, there is not enough memory to host %lu bytes\n", N * sizeof(double) );
      return 1;
    }

  // just give notice of what will happen and get the number of threads used
 #ifndef _OPENMP
  printf("serial summation\n");
 #else
 #pragma omp parallel
 #pragma omp master
  nthreads = omp_get_num_threads();
  
  printf("omp summation with %d threads\n", nthreads );
 #endif
  
  double _tstart = CPU_TIME_th;
  // initialize the array
  for ( int ii = 0; ii < N; ii++ )
    array[ii] = (double)ii;                                 // initialize the array
  
  double _tend = CPU_TIME_th;
  printf("init time is %g\n", _tend - _tstart);

  /*  -----------------------------------------------------------------------------
   *   calculate
   *  -----------------------------------------------------------------------------
   */


  double S           = 0;                                   // this will store the summation
  double th_avg_time = 0;                                   // this will be the average thread runtime
  double th_min_time = 1e11;                                // this will be the min thread runtime.
							    // contrasting the average and the min
							    // time taken by the threads, you may
							    // have an idea of the unbalance.

  double tstart  = CPU_TIME;
  
 #if !defined(_OPENMP)
  
  for ( int ii = 0; ii < N; ii++ )                          // well, you may notice this implementation
    S += array[ii];                                         // is particularly inefficient anyway

 #else

  
 #pragma omp parallel reduction(+:th_avg_time)				\
  reduction(min:th_min_time)                                // in this region there are 2 different
  {                                                         // reductions: the one of runtime, which
    struct  timespec myts;                                  // happens in the whole parallel region;
    double mystart = CPU_TIME_th;                           // and the one on S, which takes place  
   #pragma omp for reduction(+:S)                              // in the for loop.                     
    for ( int ii = 0; ii < N; ii++ )
      S += array[ii];

    th_avg_time   = CPU_TIME_th - mystart; 
    th_min_time   = CPU_TIME_th - mystart;     
  }

 #endif

  double tend = CPU_TIME;                                   // this timer is CLOCK_REALTIME if OpenMP
							    // is active; CLOCK_PROCESS_CPU_TIME_ID
							    // otherwise. That is because the latter
							    // would accounts for the whole cpu time
							    // used by the threads under OpenMP.

  /*  -----------------------------------------------------------------------------
   *   finalize
   *  -----------------------------------------------------------------------------
   */

  printf("Sum is %g, process took %g of wall-clock time\n\n"
       "<%g> sec of avg thread-time\n"
       "<%g> sec of min thread-time\n",
       S, tend - tstart, th_avg_time/nthreads, th_min_time );
  
  free( array );
  return 0;
}
