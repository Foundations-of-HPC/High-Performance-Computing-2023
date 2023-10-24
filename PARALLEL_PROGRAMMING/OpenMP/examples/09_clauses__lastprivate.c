
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

#define CPU_TIME ({struct timespec ts; double t;			\
		   clock_gettime( CLOCK_THREAD_CPUTIME_ID, &ts ),	\
		     t = (double)ts.tv_sec +				\
		     (double)ts.tv_nsec * 1e-9; t;})

#define DEFAULT   100000
#define THRESHOLD 0.1

int  valid_data  ( void );
void receive_data( double *, double *);
  

int main( int argc, char **argv )
{
  int    N = ( argc > 1 ? atoi(*(argv+1)) : DEFAULT);
  double data = 0;
  double last_time = CPU_TIME;
  srand48(time(NULL));    // initialize pseudo-random
			  // generation 
  
  printf("before the parallel region last_time (at %p) is %g\n\n",
	 &last_time, last_time );
  
 #pragma omp parallel 
  {
    int me = omp_get_thread_num();

    // here you will get that last_time is shared
    // because &last time is the same for all threads
    printf("thread %d: last_time (at %p) is %g\n", me, &last_time, last_time);

   #pragma omp barrier

   #pragma omp master
    printf("entering the for loop..\n");
    
   #pragma omp for lastprivate( last_time, data )
    for( int j = 0; j < N; j++ )
      {	
	if( valid_data() )
	  receive_data( &data, &last_time);

	// uncommenting the next line will reveal that
	// inside the for loop last_time is private
	// (better to use a small N in this case)
	// 
	/* printf(" + thread %d, iter %d: last_time (at %p) is %g\n", me, j, &last_time, last_time); */

	// !! NOTE !!
	// drand48() is not a re-entrant function, meaning that it is NOT thread-safe
	// there are different functions to be used in order to generate psuedo-random
	// sequences in multi-threading, however we'll see them in the future and
	// for the sake of simplicity we just used drand48() here.
	//
      }
  }

  printf("\nthe last valid reception %g happened at %g\n", data, last_time);
  return 0;
}


int valid_data( void )
{
  return drand48() > THRESHOLD;
}

void receive_data ( double *d, double *t)
{  
  *d = drand48();
  *t = CPU_TIME;
}
