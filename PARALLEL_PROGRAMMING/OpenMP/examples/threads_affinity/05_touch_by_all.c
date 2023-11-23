
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
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sched.h>
#include <omp.h>


#define N_default 1000

#if defined(_OPENMP)
#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
		  (double)ts.tv_nsec * 1e-9)

#define CPU_TIME_th (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ), (double)myts.tv_sec +	\
		     (double)myts.tv_nsec * 1e-9)

#else

#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)

#define CPU_TIME_th (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
		  (double)ts.tv_nsec * 1e-9)
#endif

#ifdef OUTPUT
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

#define CPU_ID_ENTRY_IN_PROCSTAT 39
#define HOSTNAME_MAX_LENGTH      200

int read_proc__self_stat ( int, int * );
int get_cpu_id           ( void       );



int main( int argc, char **argv )
{

  int     N        = N_default;
  int     nthreads = 1;
  
  struct  timespec ts, myts;
  double *array;

  /*  -----------------------------------------------------------------------------
   *   initialize 
   *  -----------------------------------------------------------------------------
   */

  // check whether some arg has been passed on
  if ( argc > 1 )
    N = atoi( *(argv+1) );

  if ( (array = (double*)malloc( N * sizeof(double) )) == NULL ) {
    printf("I'm sorry, on some thread there is not"
	   "enough memory to host %lu bytes\n",
	   N * sizeof(double) ); return 1; }
  
  // just give notice of what will happen and get the number of threads used
 #if defined(_OPENMP)  
 #pragma omp parallel
  {
 #pragma omp master
    {
      nthreads = omp_get_num_threads();
      PRINTF("omp summation with %d threads\n", nthreads );
    }
    int me = omp_get_thread_num();
 #pragma omp critical
    PRINTF("thread %2d is running on core %2d\n", me, get_cpu_id() );    
  }
 #endif

  // initialize the array;
  // each thread is "touching"
  // its own memory as long as
  // the parallel for has the
  // scheduling as the final one

  double _tstart = CPU_TIME_th;
 #pragma omp parallel for
  for ( int ii = 0; ii < N; ii++ )
    array[ii] = (double)ii;
  double _tend = CPU_TIME_th;

  printf("init takes %g\n", _tend - _tstart);
  /*  -----------------------------------------------------------------------------
   *   calculate
   *  -----------------------------------------------------------------------------
   */


  double S           = 0;                                   // this will store the summation
  double th_avg_time = 0;                                   // this will be the average thread runtime
  double th_min_time = 1e11;                                // this will be the min thread runtime.
							    // contrasting the average and the min
							    // time taken by the threads, you may
							    // have an idea of the unbalance.
  
  double tstart  = CPU_TIME;

 #if !defined(_OPENMP)
  
  for ( int ii = 0; ii < N; ii++ )                          // well, you may notice this implementation
    S += array[ii];                                         // is particularly inefficient anyway

 #else

 #pragma omp parallel reduction(+:th_avg_time)				\
  reduction(min:th_min_time)                                // in this region there are 2 different
  {                                                         // reductions: the one of runtime, which
    struct  timespec myts;                                  // happens in the whole parallel region;
    double mystart = CPU_TIME_th;                           // and the one on S, which takes place  
   #pragma omp for reduction(+:S)                              // in the for loop.                     
    for ( int ii = 0; ii < N; ii++ )
      S += array[ii];

    th_avg_time  = CPU_TIME_th - mystart; 
    th_min_time  = CPU_TIME_th - mystart; 
  }

 #endif
  
  double tend = CPU_TIME;


  /*  -----------------------------------------------------------------------------
   *   finalize
   *  -----------------------------------------------------------------------------
   */

  printf("Sum is %g, process took %g of wall-clock time\n\n",
	 S, tend - tstart);
 #if defined(_OPENMP)
  printf("<%g> sec of avg thread-time\n"
	 "<%g> sec of min thread-time\n",
	 th_avg_time/nthreads, th_min_time );
 #endif

  free( array );
 
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

