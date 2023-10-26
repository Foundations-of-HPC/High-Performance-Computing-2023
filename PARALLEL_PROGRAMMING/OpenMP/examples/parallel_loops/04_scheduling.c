
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


#define STATIC  0
#define DYNAMIC 1
#define GUIDED  2

double heavy_work( int N );


int main( int argc, char **argv )
{

  double S = 0;
  int nthreads;
  int N = 1000;

  if ( argc > 1 )
    N = atoi( *(argv+1) );



  
 #pragma omp parallel
 #pragma omp master
  nthreads = omp_get_num_threads();
  
  printf("using %d threads\n\n", nthreads);
  
  double timings[3][nthreads];
  double min_timings[3];



  memset( timings, 0, 3*nthreads*sizeof(double));
  memset( min_timings, 0, 3*sizeof(double));
  
  // ----------------------------------------------------- constant work
 #pragma omp parallel 
  {
    int myid = omp_get_thread_num();
    double tstart, tend;

    tstart = omp_get_wtime();
   #pragma omp for schedule(static) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( 1000 );
    tend = omp_get_wtime();
    timings[STATIC][myid] = tend - tstart;

    tstart = omp_get_wtime();
   #pragma omp for schedule(dynamic) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( 1000 );
    tend = omp_get_wtime();
    timings[DYNAMIC][myid] = tend - tstart;
    
    tstart = omp_get_wtime();
   #pragma omp for schedule(guided) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( 1000 );
    tend = omp_get_wtime();
    timings[GUIDED][myid] = tend - tstart;
  }


  for ( int j = 0; j < 3; j++ )
    {
      min_timings[j] = timings[j][0];
      for( int i = 1; i < nthreads; i++)
	{
	  timings[j][0] += timings[j][i];
	  min_timings[j] = (min_timings[j] < timings[j][i]) ? min_timings[j] : timings[j][i];
	}
      timings[j][0] /= nthreads;
    }
  printf("\ntimings for the constant work case:\n"
	 "\tSTATIC:  %9.7g  [ min: %9.7g ]\n"
	 "\tDYNAMIC: %9.7g  [ min: %9.7g ]\n"
	 "\tGUIDED:  %9.7g  [ min: %9.7g ]\n",
	 timings[STATIC][0], min_timings[STATIC],
	 timings[DYNAMIC][0], min_timings[DYNAMIC],
	 timings[GUIDED][0], min_timings[GUIDED]);


  // ----------------------------------------------------- increasing work  

  memset( timings, 0, 3*nthreads*sizeof(double));
  memset( min_timings, 0, 3*sizeof(double));

 #pragma omp parallel 
  {
    int myid = omp_get_thread_num();
    double tstart, tend;

    tstart = omp_get_wtime();
   #pragma omp for schedule(static) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( i );
    tend = omp_get_wtime();
    timings[STATIC][myid] = tend - tstart;

    tstart = omp_get_wtime();
   #pragma omp for schedule(dynamic) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( i );
    tend = omp_get_wtime();
    timings[DYNAMIC][myid] = tend - tstart;
    
    tstart = omp_get_wtime();
   #pragma omp for schedule(guided) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( i );
    tend = omp_get_wtime();
    timings[GUIDED][myid] = tend - tstart;
    
  }

  for ( int j = 0; j < 3; j++ )
    {
      min_timings[j] = timings[j][0];
      for( int i = 1; i < nthreads; i++)
	{
	  timings[j][0] += timings[j][i];
	  min_timings[j] = (min_timings[j] < timings[j][i]) ? min_timings[j] : timings[j][i];
	}
      timings[j][0] /= nthreads;
    }
  printf("\ntimings for the increasing work case:\n"
	 "\tSTATIC:  %9.7g  [ min: %9.7g ]\n"
	 "\tDYNAMIC: %9.7g  [ min: %9.7g ]\n"
	 "\tGUIDED:  %9.7g  [ min: %9.7g ]\n",
	 timings[STATIC][0], min_timings[STATIC],
	 timings[DYNAMIC][0], min_timings[DYNAMIC],
	 timings[GUIDED][0], min_timings[GUIDED]);


  // ----------------------------------------------------- decreasing work  
  memset( timings, 0, 3*nthreads*sizeof(double));
  memset( min_timings, 0, 3*sizeof(double));

  #pragma omp parallel 
  {
    int myid = omp_get_thread_num();
    double tstart, tend;

    tstart = omp_get_wtime();
   #pragma omp for schedule(static) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( N-i+1 );
    tend = omp_get_wtime();
    timings[STATIC][myid] = tend - tstart;

    tstart = omp_get_wtime();
   #pragma omp for schedule(dynamic) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( N-1+1 );
    tend = omp_get_wtime();
    timings[DYNAMIC][myid] = tend - tstart;
    
    tstart = omp_get_wtime();
   #pragma omp for schedule(guided) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( N-i+1 );
    tend = omp_get_wtime();
    timings[GUIDED][myid] = tend - tstart;    
  }


  for ( int j = 0; j < 3; j++ )
    {
      min_timings[j] = timings[j][0];
      for( int i = 1; i < nthreads; i++)
	{
	  timings[j][0] += timings[j][i];
	  min_timings[j] = (min_timings[j] < timings[j][i]) ? min_timings[j] : timings[j][i];
	}
      timings[j][0] /= nthreads;
    }  
  printf("\ntimings for the decreasing work case:\n"
	 "\tSTATIC:  %9.7g  [ min: %9.7g ]\n"
	 "\tDYNAMIC: %9.7g  [ min: %9.7g ]\n"
	 "\tGUIDED:  %9.7g  [ min: %9.7g ]\n",
	 timings[STATIC][0], min_timings[STATIC],
	 timings[DYNAMIC][0], min_timings[DYNAMIC],
	 timings[GUIDED][0], min_timings[GUIDED]);


  // ----------------------------------------------------- random work  
  memset( timings, 0, 3*nthreads*sizeof(double));
  memset( min_timings, 0, 3*sizeof(double));

 #pragma omp parallel 
  {
    int myid   = omp_get_thread_num();
    int myseed = myid;
    double tstart, tend;

    srand( myseed );
    tstart = omp_get_wtime();
   #pragma omp for schedule(static) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( 100 + rand_r(&myseed) % 2345 );
    tend = omp_get_wtime();
    timings[STATIC][myid] = tend - tstart;

    srand( myseed );
    tstart = omp_get_wtime();
   #pragma omp for schedule(dynamic) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( 100 + rand_r(&myseed) % 2345 );
    tend = omp_get_wtime();
    timings[DYNAMIC][myid] = tend - tstart;

    srand( myseed );
    tstart = omp_get_wtime();
   #pragma omp for schedule(guided) reduction(+:S)
    for( int i = 0; i < N; i++ )
      S += heavy_work( 100 + rand_r(&myseed) % 2345 );
    tend = omp_get_wtime();
    timings[GUIDED][myid] = tend - tstart;
    
  }

  
    for ( int j = 0; j < 3; j++ )
    {
      min_timings[j] = timings[j][0];
      for( int i = 1; i < nthreads; i++)
	{
	  timings[j][0] += timings[j][i];
	  min_timings[j] = (min_timings[j] < timings[j][i]) ? min_timings[j] : timings[j][i];
	}
      timings[j][0] /= nthreads;
    }
  printf("\ntimings for the random work case:\n"
	 "\tSTATIC:  %9.7g  [ min: %9.7g ]\n"
	 "\tDYNAMIC: %9.7g  [ min: %9.7g ]\n"
	 "\tGUIDED:  %9.7g  [ min: %9.7g ]\n",
	 timings[STATIC][0], min_timings[STATIC],
	 timings[DYNAMIC][0], min_timings[DYNAMIC],
	 timings[GUIDED][0], min_timings[GUIDED]);

  return 0;
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
