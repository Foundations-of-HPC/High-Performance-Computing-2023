
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


#define MAX_DATA  (1<<15)     // just a number significantly larger
			      // than any expected number of cores
			      // on a NUMA node
#define THRESHOLD 2000

int    more_data_arriving( int );
int    getting_data( int ** );
double heavy_work( int );


int main ( int argc, char **argv )
{

  srand48(time(NULL));

  int  Nthreads;
  int  iteration = 0;
  int  data_are_arriving;
  int  ndata;
  int  bunch;
  int  next_bunch = 0;
  int *data; 

  bunch = (argc > 1 ? atoi(*(argv+1)) : 10 );
  
 #pragma omp parallel firstprivate(bunch)
  {
    int me = omp_get_thread_num();
    
   #pragma omp single
    {
      Nthreads = omp_get_num_threads();
      data_are_arriving = more_data_arriving(0);
    }

    
    while( data_are_arriving )
      {

       #pragma omp single                               // [1] a thread is getting data
	{
	  ndata = getting_data( &data );
	  printf("iteration %d: thread %d got %d data\n",
		 iteration, me, ndata );
	}
	

	int mystart;
	do
	  {
	    int mystop;
	   #pragma omp atomic capture
	    { mystart = next_bunch; next_bunch += bunch; }
	    
	    if( mystart < ndata ) {
	      mystop = mystart + bunch;
	      mystop = (mystop > ndata ? ndata : mystop);
	     #if defined(DETAILS)
	      printf("\tthread %d processing [%d:%d]\n",
		     me, mystart, mystop);
	     #endif
	      for( ; mystart < mystop; mystart++ )
		heavy_work( mystart ); }
	  } while( mystart < ndata );


       #pragma omp barrier		             // this barrier is needed because
						     // in the following single region
						     // the shared memory data is freed.
						     // Then we must ensure that the threads
						     // that enters the single region does
						     // not free the data while another thread
						     // is still in the while loop.
						     // Is there any different way in which
						     // we can implement this?
	                   
       #pragma omp single     	                     // [4] somebody updates the iteration
	{
	  while( nthreads_that_finished < nthreads); //     and calls more_data_arriving()
	  free( data );
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


int getting_data( int **data )
{
 #define MIN  10
 #define MAX 25
  
  // produces no more than n-1
  // data
  int howmany = lrand48() % MAX_DATA;
  howmany = ( howmany == 0 ? 1 : howmany);

  // be sure that the data
  // array has enough room
  // to host up to n-1 data
  *data = (int*)calloc( howmany, sizeof(int));
  
  for( int j = 0; j < howmany; j++ )
    (*data)[j] = 1024 + lrand48() % (MAX-MIN);  // values will range
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
