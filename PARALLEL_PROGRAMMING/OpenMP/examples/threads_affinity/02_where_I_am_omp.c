
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
#if !defined(_OPENMP)
#error "OpenMP support is mandatory for this code"
#endif
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>


#define FORMAT_SIZE 200
#define AFFINITY_SIZE 200

int main( int argc, char **argv )
{

  int nthreads           = 1;
  int nthreads_requested = 1;
  

  if ( argc > 1 )
    nthreads_requested = atoi( *(argv+1) );

  if ( nthreads_requested > 1 )
    omp_set_num_threads( nthreads_requested ); 

  char *places = getenv("OMP_PLACES");
  char *bind   = getenv("OMP_PROC_BIND");
  if ( places != NULL )
    printf("OMP_PLACES is set to %s\n", places);
  if ( bind != NULL )
    printf("OMP_PROC_BINDING is set to %s\n", bind);


  {
    char affinity_format[FORMAT_SIZE];
    char affinity_myformat[] = "host is \"%30H\", my id is %0.3n, bound to %A";

    int n = omp_get_affinity_format( affinity_format, FORMAT_SIZE);
    printf("default affinity format is « %s »\n", affinity_format);
    if ( n > FORMAT_SIZE )
      printf("[ affinity format has been truncated, %d characters have been left out\n",
	     n - FORMAT_SIZE);

    omp_set_affinity_format( affinity_myformat );
  }

  
  int    nbuffers   = omp_get_num_procs();
  char **affinities = (char**)malloc( nbuffers * sizeof(char*) );
  for ( int i = 0; i < nbuffers; i++ ) affinities[i] = (char*)malloc( AFFINITY_SIZE * sizeof(char) );

  int truncations = 0;
 #pragma omp parallel reduction(max:truncations)
  {
    
   #pragma omp master
    {
      char *proc_bind_names[] = { "false (no binding)",
				  "true",
				  "master",
				  "close",
				  "spread" };
      
      nthreads = omp_get_num_threads();
      
      // get the current binding
      int binding = omp_get_proc_bind();

      printf("+ %d threads in execution - - proc bind is set to \"%s\"\n",
	     nthreads, proc_bind_names[binding] );

      if ( nthreads > nbuffers )
	exit(1);
    }
    int me      = omp_get_thread_num();
    
    int n = omp_capture_affinity( affinities[me], (size_t)AFFINITY_SIZE, NULL );
    truncations = ( n > AFFINITY_SIZE ? n : 0 );
    
   #ifdef SPY
   #define REPETITIONS 10000
   #define ALOT        10000000000
    long double S = 0;
    for( int j = 0; j < REPETITIONS; j++ )
     #pragma omp for
      for( unsigned long long i = 0; i < ALOT; i++ )
	S += (long double)i;
   #endif
  }

  for ( int i = 0; i <  nbuffers; i++ )
    {
      printf("%s\n", affinities[i] );
      free( affinities[i] );
    }
  free( affinities );

  if ( truncations > 0 )
    printf("[ warning: some of the affinities were truncated ]\n" );
  
  return 0;
}







