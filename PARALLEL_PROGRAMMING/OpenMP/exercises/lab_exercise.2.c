
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


#define THRESHOLD 2000

int    more_data_arriving( int );
int    getting_data( int, int * );
double heavy_work( int );


int main ( int argc, char **argv )
{

  srand48(time(NULL));


  // you are free to decide which
  // variable are shared and (thread)private
  //


  // ....
  //
  
 #pragma omp parallel
  {
    while( data_are_arriving )       // data_are_arriving should be
				     // an int whose value is known
				     // to everybody
      {

        // you must account for how many
        // iteration have been done in
	// this loop
	
	// [1] here somebody receives
	//     the data (and how many
	//     they are).
	//     Assume that the number
	//     of data are in the range
	//     [1..Nthreads]
	//     "data" is an array of
	//     integers that is generated
	//     by getting_data()

	// [2] threads process the data
	//     so that each thread process
	//     one.
	//     "processing" means to call
	//     the function heavy_work()
	//     using as argument an entry
	//     of the int array generated
	//     by getting_data() at step [1]
	//
	//     All the data must be processed
	//     before getting to the next
	//     iteration

	// [3] somebody get whether new
	//     data will be arriving by
	//     calling more_data_arriving().
	//     The argument of more_data_arriving()
	//     is the iteration number

	// [4] somebody updates the iteration
	//     number
    
      }
    
  }
  
  return 0;
}


int more_data_arriving( int i )
{
  // it is increasingly probable that
  // no more data arrive when i approaches
  // THRESHOLD
  //
  double p = (double)(THRESHOLD - i) / THRESHOLD;
  return (drand48() < p);
}


int getting_data( int n, int *data )
{
 #define MIN  1000
 #define MAX 10000
  
  // produces no more than n-1
  // data
  int howmany = lrand48() % n;
  howmany = ( howmany == 0 ? 1 : howmany);

  // be sure that the data
  // array has enough room
  // to host up to n-1 data
  for( int j = 0; j < howmany; j++ )
    data[j] = 1024 + lrand48() % (MAX-MIN);  // values will range
				             // from MIN up to MAX
  
  return howmany;
}

double heavy_work( int N )
{
  double guess = 3.141572 / 3 * N;

  for( int i = 0; i < N; i++ )
    {
      guess = exp( guess );
      guess = sin( guess );

    }
  return guess;
}
