
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


int main( int argc, char **argv )
{

  int nthreads;

#if defined(_OPENMP)
  
#pragma omp parallel               // this creates a parallel region
                                   // that is encompassed by the
                                   // opening and closing { }
                                   //
                                   // you can modify the number of
                                   // spawned threads through the
                                   //   OMP_THREAD_NUM
                                   // environmental variable
  
  {   
    
    int my_thread_id = omp_get_thread_num();  // note: this assignment is now
                                              // thread-safe because the lvalue
					      // is a private variable
    #pragma omp master
    nthreads = omp_get_num_threads();

                                   // the order in which different threads will
                                   // arrive at this print is undefined;
                                   // if you run this code several times, you will
                                   // obtain different results

    printf( "\tgreetings from thread num %d\n", my_thread_id);
  }
#else
  
  nthreads = 1;
  printf( "\tgreetings from thread num 0\n");
#endif
  
  printf(" %d thread%s greeted you from the %sparallel region\n",
	nthreads, (nthreads==1)?" has":"s have", (nthreads==1)?"(non)":"" );
  
  return 0;
}
