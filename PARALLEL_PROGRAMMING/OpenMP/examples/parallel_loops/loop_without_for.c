
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
#include <omp.h>


#define N_default     1000  // how long is the main array

int main( int argc, char **argv )
{

  int N         = N_default;
  int nthreads  = 1;

  // check whether some arg has been passed on
  if ( argc > 1 )
    {
      N = atoi( *(argv+1) );
      if ( argc > 2 )
	nthreads = atoi( *(argv+2) );
    }

  if( nthreads > 1 )
    omp_set_num_threads(nthreads);
  #pragma omp parallel
  {
    int me       = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    
    int chunk    = N / nthreads;
    int mod      = N % nthreads;
    int my_first = chunk*me + ((me < mod)?me:mod);
    int my_chunk = chunk + (mod > 0)*(me < mod);

   #pragma omp single
    printf("nthreads: %d, N: %d --- chunk is %d, reminder is %d\n", nthreads, N, chunk, mod);

    printf("thread %d : from %d to %d\n", me, my_first, my_first+my_chunk);

    /*
     * here you could then insert a for loop
     * int my_stop = my_first + my_chunk;
     * for( int i = myfirst; i < my_stop; i++ )
     *   ...
     */
  } 


  return 0;
}

