
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
#include <math.h>
#include <omp.h>


#define N_DFLT 1000


int main ( int argc, char **argv )
{

  int N   = ( (argc > 1) ? atoi(*(argv+1)) : N_DFLT);
  int Nth = ( (argc > 2) ? atoi(*(argv+2)) : 0);

  unsigned int *array = (int*)malloc( sizeof(int) * N );

  if ( Nth > 0 )
    omp_set_num_threads = Nth;
  
 #pragma omp parallel
  {
    int myid     = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    
    for ( unsigned int i = 0; i < N; i++ )
      array[i] = i*i;

  }

  //
  // check the results
  // can you parallelize this as well ?
  //
  
  unsigned int faults = 0;
  for ( unsigned int i = 0; i < N; i++ )
    faults += ( array[i] != i*i );

  if ( faults > 0 )
    printf("wow, you've been able to get %u faults\n",
	   faults );
  
  return 0;
}
