
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

  int  Nthreads;
  int  iteration = 0;
  int  data_are_arriving;
  int  ndata;
  int *data; 
  
 #pragma omp parallel
  {
    int me = omp_get_thread_num();
    
   #pragma omp single
    {
      Nthreads = omp_get_num_threads();
      data     = (int*)calloc(Nthreads, sizeof(int));   // requirements specify that
							// there will never be more
							// than Nthreads data
      data_are_arriving = more_data_arriving(0);
    }

    
    while( data_are_arriving )
      {

       #pragma omp single                               // [1] a thread is getting data
	{
	  ndata = getting_data( Nthreads, data );
	  printf("iteration %d: thread %d got %d data\n",
		 iteration, me, ndata );
	}
	

	if( me < ndata )                                // [2] threads process the data
	  {                                             //     the simplest subdivision is
							//     that each one process the
	    heavy_work( data[me] );                     //     entry corresponding to its
	  }                                             //     id.
	    

       #pragma omp single     	                        // [4] somebody updates the iteration
	{                                               //     and calls more_data_arriving()
	  if( !(data_are_arriving = more_data_arriving(iteration+1)) )
	    printf("\t>>> iteration %d : thread %d got the news that "
		   "no more data will arrive\n",
		   iteration, me);
	  else
	    iteration++;                                // in a single region we do not need
	                                                // an atomic op
	}
    
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
