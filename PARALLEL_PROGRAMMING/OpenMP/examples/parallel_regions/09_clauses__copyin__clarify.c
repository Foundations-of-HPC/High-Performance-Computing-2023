
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


double golden_value = 0;
#pragma omp threadprivate( golden_value )


int main( int argc, char **argv )
{
  srand48(time(NULL));
  int N = 10;  

 #pragma omp parallel copyin(golden_value)
  // the copying of thread 0's golden_value
  // happens here, at the entering of the
  // parallel region;
  //
  {
    
   #pragma omp master
    golden_value = 1.618033988;       // we do not expect
				      // this value to be
				      // broadcasted

   #pragma omp barrier
    
    printf("[PR 1] thread %d has a golden value %g\n",
	   omp_get_thread_num(), golden_value );
  }    


 #pragma omp parallel copyin(golden_value)
  // here the master's value is copied again;
  // since it was modified in the previous
  // PR, we do expect that now everybody
  // will have the new value
  //
  printf("[PR 2] thread %d has a golden value %g\n",
	 omp_get_thread_num(), golden_value );

  return 0;
}
