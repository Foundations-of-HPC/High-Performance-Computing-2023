
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

void do_something( int who_am_I )
{
 #pragma omp ordered
  printf( "\tgreetings from thread num %d\n", who_am_I );
}


int main( int argc, char **argv )
{

  int nthreads;
  
 #if defined(_OPENMP)

 #pragma omp parallel
  {   
    
    int my_thread_id = omp_get_thread_num();
   #pragma omp master
    nthreads = omp_get_num_threads();
   #pragma omp barrier                           // let all the threads to read
						 //   the correct value of nthreads
    
   #pragma omp for ordered                       // declare a for within which there
    for ( int i = 0; i < nthreads; i++)          //   are ordered regions
      do_something( my_thread_id );
      

  }
 #else
  
  nthreads = 1;
 #endif
  
  printf(" %d thread%s greeted you from the %sparallel region\n", nthreads, (nthreads==1)?" has":"s have", (nthreads==1)?"(non)":"" );
  
  return 0;
}
