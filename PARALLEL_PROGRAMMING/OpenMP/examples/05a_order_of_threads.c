
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

  int order = 0;
  
 #pragma omp parallel               // this creates a parallel region
                                   // that is encompassed by the
                                   // opening and closing { }
                                   //
                                   // you can modify the number of
                                   // spawned threads through the
                                   //   OMP_THREAD_NUM
                                   // environmental variable
  
  {   
    
    int my_thread_id = omp_get_thread_num();
   #pragma omp master
    nthreads = omp_get_num_threads();

                                   // now we impose an ordered output
                                   // although not in a very efficient way
    int done = 0;
    while (!done)
      {
        	                   // the "critical" directive identifies a
        	                   // section that must be executed by a
        	                   // single thread at a time
	                           // we need to enclose it within the while
	                           // loop, because otherwise un unspecified
	                           // number of threads will print the msg.
	                           // That is just due to this particular
	                           // case: in fact, ALL the threads will
	                           // execute the if test. However, which are
	                           // those that succeed, print and modify the
	                           // "order" value depends on which have been
	                           // the previous ones and on the delay.
       #pragma omp critical 
	if ( order == my_thread_id )
	  {
	    printf( "\tgreetings from thread num %d\n", my_thread_id );
	    order++;
	    done = 1;
	  }
      }
  }
 #else
  
  nthreads = 1;
 #endif
  
  printf(" %d thread%s greeted you from the %sparallel region\n", nthreads, (nthreads==1)?" has":"s have", (nthreads==1)?"(non)":"" );
  
  return 0;
}
