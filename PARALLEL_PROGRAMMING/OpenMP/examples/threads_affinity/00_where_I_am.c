
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
#error "OpenMP support required for this code"
#endif
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <omp.h>


#define N_default 1000


#define CPU_ID_ENTRY_IN_PROCSTAT 39
#define HOSTNAME_MAX_LENGTH      200

int read_proc__self_stat ( int, int * );
int get_cpu_id           ( void       );

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
  
 #pragma omp parallel
  {
    
   #pragma omp single 
    {
      nthreads = omp_get_num_threads();
      printf("+ %d threads in execution - -\n", nthreads );
    }
    int me = omp_get_thread_num();
    
   #pragma omp critical
    // we use critical only for having not-interleaved lines of output
    printf("thread %2d is running on core %2d\n", me, get_cpu_id() );

   #pragma omp barrier

   #ifdef SPY
    // if you compile with -DSPY then you can
    // inspect what is happening by running htop
    //
   #define REPETITIONS 10000
   #define ALOT        10000000000
    long double S = 0;
    for( int j = 0; j < REPETITIONS; j++ )
     #pragma omp for
      for( unsigned long long i = 0; i < ALOT; i++ )
	S += (long double)i;
   #endif
  }

  return 0;
}






int get_cpu_id( void )
{
 #if defined(_GNU_SOURCE)                              // GNU SOURCE ------------
  
  return  sched_getcpu( );

 #else

 #ifdef SYS_getcpu                                     //     direct sys call ---
  
  int cpuid;
  if ( syscall( SYS_getcpu, &cpuid, NULL, NULL ) == -1 )
    return -1;
  else
    return cpuid;
  
 #else      

  unsigned val;
  if ( read_proc__self_stat( CPU_ID_ENTRY_IN_PROCSTAT, &val ) == -1 )
    return -1;

  return (int)val;

 #endif                                                // -----------------------
 #endif

}



int read_proc__self_stat( int field, int *ret_val )
/*
  Other interesting fields:

  pid      : 0
  father   : 1
  utime    : 13
  cutime   : 14
  nthreads : 18
  rss      : 22
  cpuid    : 39

  read man /proc page for fully detailed infos
*/
{
  // not used, just mnemonic
  // char *table[ 52 ] = { [0]="pid", [1]="father", [13]="utime", [14]="cutime", [18]="nthreads", [22]="rss", [38]="cpuid"};

  *ret_val = 0;

  FILE *file = fopen( "/proc/self/stat", "r" );
  if (file == NULL )
    return -1;

  char   *line = NULL;
  int     ret;
  size_t  len;
  ret = getline( &line, &len, file );
  fclose(file);

  if( ret == -1 )
    return -1;

  char *savetoken = line;
  char *token = strtok_r( line, " ", &savetoken);
  --field;
  do { token = strtok_r( NULL, " ", &savetoken); field--; } while( field );

  *ret_val = atoi(token);

  free(line);

  return 0;
}

