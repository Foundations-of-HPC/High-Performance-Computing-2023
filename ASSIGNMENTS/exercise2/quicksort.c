
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
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <omp.h>
#include <time.h>



// ================================================================
//  MACROS and DATATYPES
// ================================================================


#if defined(_OPENMP)

// measure the wall-clock time
#define CPU_TIME (clock_gettime( CLOCK_REALTIME, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)

// measure the cpu thread time
#define CPU_TIME_th (clock_gettime( CLOCK_THREAD_CPUTIME_ID, &myts ), (double)myts.tv_sec +     \
                     (double)myts.tv_nsec * 1e-9)

#else

// measure ther cpu process time
#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec + \
                  (double)ts.tv_nsec * 1e-9)
#endif


#if defined(DEBUG)
#define VERBOSE
#endif

#if defined(VERBOSE)
#define PRINTF(...) printf(__VA_ARGS__)
#else
#define PRINTF(...)
#endif

//
// The data to be sorted consists in a structure, just to mimic that you
// may have to sort not basic types, whch may have some effects on the memory efficiency.
// The structure, defined below as data_t is just an array of DATA_SIZE double,
// where DATA_SDIZE is defined here below.
// However, for the sake of simplicity, we assume that we sort with respect one of
// the double in each structure, the HOTst one, where HOT is defined below (my
// choice was to set HOT to 0, so the code will use the first double to sort the
// data).
//
// Note that you can chnage the data structure and the ordering as you like.

#if !defined(DATA_SIZE)
#define DATA_SIZE 8
#endif
#define HOT       0

// let's define the default amount of data
//
#if (!defined(DEBUG) || defined(_OPENMP))
#define N_dflt    100000
#else
#define N_dflt    10000
#endif


// let's define the data_t type
//
typedef struct
{
  double data[DATA_SIZE];
} data_t;


// let's defined convenient macros for max and min between
// two data_t objects
//
#define MAX( a, b ) ( (a)->data[HOT] >(b)->data[HOT]? (a) : (b) );
#define MIN( a, b ) ( (a)->data[HOT] <(b)->data[HOT]? (a) : (b) );


// ================================================================
//  PROTOTYPES
// ================================================================

// let'ìs define the compare funciton that will be used by the
// sorting routine
//
typedef int (compare_t)(const void*, const void*);

// let's define the verifying function type, used to test the
// results
//
typedef int (verify_t)(data_t *, int, int, int);


// declare the funcitons
//
extern inline compare_t compare;        // the compare function
extern inline compare_t compare_ge;     // the compare for "greater or equal"
verify_t  verify_partitioning;          // verification functions
verify_t  verify_sorting;
verify_t  show_array;

// declare the partitioning function
//
extern inline int partitioning( data_t *, int, int, compare_t );

// declare the sorting function
//
void qsort( data_t *, int, int, compare_t ); 


// ================================================================
//  CODE
// ================================================================


int main ( int argc, char **argv )
{

  
  // ---------------------------------------------
  //  get the arguments
  //


  int N          = N_dflt;
  
  /* check command-line arguments */
  {
    int a = 0;
    
    if ( argc > ++a ) N = atoi(*(argv+a));
  }
  
  // ---------------------------------------------
  //  generate the array
  //
  
  data_t *data = (data_t*)malloc(N*sizeof(data_t));
  long int seed;
 #if defined(_OPENMP)
 #pragma omp parallel
  {
    int me             = omp_get_thread_num();
    short int seed     = time(NULL) % ( (1 << sizeof(short int))-1 );
    short int seeds[3] = {seed-me, seed+me, seed+me*2};

   #pragma omp for
    for ( int i = 0; i < N; i++ )
      data[i].data[HOT] = erand48( seeds );
  }
 #else
  {
    seed = time(NULL);
    srand48(seed);
    
    PRINTF("ssed is % ld\n", seed);
    
    for ( int i = 0; i < N; i++ )
      data[i].data[HOT] = drand48();
  }    
 #endif

  
  // ---------------------------------------------
  //  process 
  //
  struct timespec ts;
  int    nthreads = 1;
  double tstart = CPU_TIME;
  
 #if defined(_OPENMP)

 #pragma omp parallel
  {


  }
  
 #else

  qsort( data, 0, N, compare_ge );
  
 #endif
  
  double tend = CPU_TIME;  
  
  // ---------------------------------------------
  //  release the memory and stop
  //

  if ( verify_sorting( data, 0, N, 0) )
    printf("%d\t%g sec\n", nthreads, tend-tstart);
  else
    printf("the array is not sorted correctly\n");
  
  free( data );

  return 0;
}


 #define SWAP(A,B,SIZE) do {int sz = (SIZE); char *a = (A); char *b = (B); \
    do { char _temp = *a;*a++ = *b;*b++ = _temp;} while (--sz);} while (0)

inline int partitioning( data_t *data, int start, int end, compare_t cmp_ge )
{
  
  // pick up the meadian of [0], [mid] and [end] as pivot
  //
  
  /* to be done */

  // pick up the last element as pivot
  //
  --end;  
  void *pivot = (void*)&data[end];

  // partition around the pivot element
  
  int pointbreak = end-1;
  for ( int i = start; i <= pointbreak; i++ )
    if( cmp_ge( (void*)&data[i], pivot ) )
      {
	while( (pointbreak > i) && cmp_ge( (void*)&data[pointbreak], pivot ) ) pointbreak--;
	if (pointbreak > i ) 
	  SWAP( (void*)&data[i], (void*)&data[pointbreak--], sizeof(data_t) );
      }  
  pointbreak += !cmp_ge( (void*)&data[pointbreak], pivot ) ;
  SWAP( (void*)&data[pointbreak], pivot, sizeof(data_t) );
  
  return pointbreak;
}


void qsort( data_t *data, int start, int end, compare_t cmp_ge )
{

 #if defined(DEBUG)
 #define CHECK {							\
    if ( verify_partitioning( data, start, end, mid ) ) {		\
      printf( "partitioning is wrong\n");				\
      printf("%4d, %4d (%4d, %g) -> %4d, %4d  +  %4d, %4d\n",		\
	     start, end, mid, data[mid].data[HOT],start, mid, mid+1, end); \
      show_array( data, start, end, 0 ); }}
 #else
 #define CHECK
 #endif

  int size = end-start;
  if ( size > 2 )
    {
      int mid = partitioning( data, start, end, cmp_ge );

      CHECK;
      
      qsort( data, start, mid, cmp_ge );    // sort the left half
      qsort( data, mid+1, end , cmp_ge );   // sort the right half
    }
  else
    {
      if ( (size == 2) && cmp_ge ( (void*)&data[start], (void*)&data[end-1] ) )
	SWAP( (void*)&data[start], (void*)&data[end-1], sizeof(data_t) );
    }
}




 
int verify_sorting( data_t *data, int start, int end, int not_used )
{
  int i = start;
  while( (++i < end) && (data[i].data[HOT] >= data[i-1].data[HOT]) );
  return ( i == end );
}

int verify_partitioning( data_t *data, int start, int end, int mid )
{
  int failure = 0;
  int fail = 0;
  
  for( int i = start; i < mid; i++ )
    if ( compare( (void*)&data[i], (void*)&data[mid] ) >= 0 )
      fail++;

  failure += fail;
  if ( fail )
    { 
      printf("failure in first half\n");
      fail = 0;
    }

  for( int i = mid+1; i < end; i++ )
    if ( compare( (void*)&data[i], (void*)&data[mid] ) < 0 )
      fail++;

  failure += fail;
  if ( fail )
    printf("failure in second half\n");

  return failure;
}


int show_array( data_t *data, int start, int end, int not_used )
{
  for ( int i = start; i < end; i++ )
    printf( "%f ", data[i].data[HOT] );
  printf("\n");
  return 0;
}


inline int compare( const void *A, const void *B )
{
  data_t *a = (data_t*)A;
  data_t *b = (data_t*)B;

  double diff = a->data[HOT] - b->data[HOT];
  return ( (diff > 0) - (diff < 0) );
}

inline int compare_ge( const void *A, const void *B )
{
  data_t *a = (data_t*)A;
  data_t *b = (data_t*)B;

  return (a->data[HOT] >= b->data[HOT]);
}
