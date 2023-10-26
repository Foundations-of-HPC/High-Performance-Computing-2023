
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


int me;
#pragma omp threadprivate(me)


void do_work( double[2], double * );


int main( int argc, char **argv )
{
  long int t = time(NULL);
  srand48(t);
  
  int N = 10;  

 #pragma omp parallel
  me = omp_get_thread_num(); 
  
  // ------------------------------------------
  //
  for ( int j = 0; j < N; j++ )
    {      
      double result = 0;
     #pragma omp parallel 
      {
	double seed[2];

	// let's generate/acquire some value needed
	// here and that can be generated/acquired
	// only by one thread
       #pragma omp single copyprivate(seed)
	seed[0] = drand48(), seed[1] = drand48();

	// notice that the copy of array-type variables
	// happens by assignment element-wise

	// remind: there is an implicit barrier at
	// the end of the single region
	//
	// now, the seed, which is private to each
	// thread, has the same value for all the threads

	// we need to modify the seed value in a
	// way which is typical of each thread;
	// that is way we don't use a shared variable
	//
	seed[0] += sqrt((double)me);
	double thread_result;
	do_work( seed, &thread_result );

       #pragma omp atomic update
	result += thread_result;
      }

      printf("iter %d: result is %g\n", j, result);
    }    

  // ------------------------------------------
  // let's find an alternative to copyprivate
  // which, in this case is quite simple
  //

  printf("\n\n");
  
  // let's initialize the random generator with
  // the same value than before so that we can
  // check that the results are the same
  //
  srand48(t);

  
  for ( int j = 0; j < N; j++ )
    {      
      double result = 0;
      double seed[2];
     #pragma omp parallel 
      {
	// let's generate/acquire some value needed
	// here and that can be generated/acquired
	// only by one thread
       #pragma omp single 
	seed[0] = drand48(), seed[1] = drand48();

	double myseed[2] = {seed[0] + sqrt((double)me),
			    seed[1]};
	double thread_result;
	do_work( myseed, &thread_result );

       #pragma omp atomic update
	result += thread_result;
      }

      printf("iter %d: result is %g\n", j, result);
    }    


  // ------------------------------------------
  
  return 0;
}



void do_work( double seed[2], double *res )
{
  double tmp = ( seed[0] > 0 ? log10(seed[0]) : sin(seed[0]) );
  *res = seed[0] * seed[1] * tmp;
  return;
}
