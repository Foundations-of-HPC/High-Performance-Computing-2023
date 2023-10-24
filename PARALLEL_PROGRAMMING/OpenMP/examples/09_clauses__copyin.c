
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


int me;
int golden_values[3];
#pragma omp threadprivate( me, golden_values )


void set_up_golden_values( void );
void do_work( int, unsigned long long *);


int main( int argc, char **argv )
{
  srand48(time(NULL));
  int N = 10;  

 #pragma omp parallel
  me = omp_get_thread_num();


  // ------------------------------------------
  //
  for ( int j = 0; j < N; j++ )
    {
      // let's suppose that we need
      // to perform a big bunch of
      // calculations in each iteration;
      // however, let's say that the
      // initial value at each iteration
      // are unpredictable, or in any
      // case that they must be generated
      // or acquired by one thread only

      set_up_golden_values( );

      unsigned long long result;
     #pragma omp parallel copyin(golden_values)
      {
	// here golden_values are initialized to
	// the values of thread 0 for all the threads
	//
	unsigned long long thread_result;
	do_work( me, &thread_result );
       #pragma omp atomic update
	result += thread_result;
      }
    }    

  // ------------------------------------------
  // let's find an alternative to copyin
  // which, in this case is quite simple
  //
  
  for ( int j = 0; j < N; j++ )
    {
      // let's suppose that we need
      // to perform a big bunch of
      // calculations in each iteration;
      // however, let's say that the
      // initial value at each iteration
      // are unpredictable, or in any
      // case that they must be generated
      // or acquired by one thread only

      set_up_golden_values( );

      int local_golden[3];
      for( int i = 0; i < 3; i++ )
	local_golden[i] = golden_values[i];
      
      unsigned long long result;
     #pragma omp parallel
      {
	for( int i = 0; i < 3; i++ )
	  golden_values[i] = local_golden[i];
	
	unsigned long long thread_result;
	do_work( me, &thread_result );
	
       #pragma omp atomic update
	result += thread_result;
      }
    }    

  // ------------------------------------------
  
  return 0;
}



void set_up_golden_values( void )
{
  // suppose that for some reason we
  // need that the random values are
  // generated only by a single thread
  //
  for( int j = 0; j < 3; j++ )	 
    golden_values[j] = lrand48();
  
  return;
}



void do_work( int who, unsigned long long *res )
{
  unsigned long long sum = 0;
  for( int j = 0; j < 3; j++ )
    golden_values[j] += who;
  for( int j = who*100; j < who*200; j++ )
    sum += (golden_values[0]*j + golden_values[1]) % golden_values[2];

  *res = sum;
  return;
}
