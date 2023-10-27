

#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 199901L)
#     define _XOPEN_SOURCE 700
#  endif
#endif
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <stdio.h>
#include <time.h>
#include "prefix_sum.serial.h"



inline double scan( const uint N, DTYPE * restrict array )
{

  DTYPE avg    = array[0];

  for ( uint ii = 1; ii < N; ii++ )
    {
      avg       += array[ii];
      array[ii]  = avg;
    }
  
  return avg;
}


inline DTYPE scan_efficient( const uint N, DTYPE * restrict array )
{

  uint N_4 = (N/4)*4;

  {
    DTYPE temp = array[2];
    array[1]   += array[0];
    array[3]   += temp;
    array[2]   += array[1];
    array[3]   += array[1];
  }
  
  PRAGMA_VECT_LOOP
  for ( uint ii = 4; ii < N_4; ii+=4 )
    {
      DTYPE register temp = array[ii+2];
      array[ii]     += array[ii-1];      
      array[ii+1]   += array[ii];
      array[ii+3]   += temp;
      array[ii+2]   += array[ii+1];
      array[ii+3]   += array[ii+1];      
    }
  
  for ( uint ii = N_4; ii < N; ii++ )
    array[ii] += array[ii-1];
  
  return array[N-1];
}


#define N_default  1000
#define _scan      0
#define _scan_e    1

int main ( int argc, char **argv )
{
  
  struct timespec ts;  
  int             Nth_level1 = 1;
  int             Nth_level2 = 0;
  
  // -------------------------------------------------------------
  // variables' initialization to default values
  //

  uint    N        = N_default;
  int    scan_type = _scan;
  
  
  if ( argc > 1 )
    {
      scan_type = atoi( *(argv+1) );
      if ( argc > 2 )
	N  = (unsigned)atoi( *(argv+2) );
    }

  printf( "scan type: %d\n", scan_type );

  
  // -------------------------------------------------------------
  // data init.

  double timing_start;
  double timing_scan;
  double timing_prepare;
  double total_weight;
  
  uint   N_alloc = ((N/4)+1)*4;
  //  DTYPE *array   = (DTYPE*)aligned_alloc( 32, N_alloc * sizeof(DTYPE) );
  DTYPE *array   = (DTYPE*)malloc( N_alloc * sizeof(DTYPE) );

  timing_start = CPU_TIME;

  // initialize with pseudo-random numbers 

  /* srand48(time(0)); */
  /* for ( int ii = 0; ii < N; ii++ ) */
  /*   topnodes[ii] = base + drand48()*range; */

  // initialize with the first N integer
  // (that makes the results easy to check)
  // //

  for ( uint ii = 0; ii < N; ii++ )
    array[ii] = (double)ii;
  
  timing_prepare = CPU_TIME - timing_start;

  // ................................................
  //  SCAN
  // ................................................

  if ( scan_type == _scan )
    total_weight = scan( N, array );

  else if (scan_type == _scan_e)
    total_weight = scan_efficient( N, array );

  /* else if (scan_type == _scan_b) */
  /*   total_weight = scan_b( N, array ); */

  timing_scan  = CPU_TIME - timing_start;      

  printf("timing for scan is %g, timing for prepare is %g [total weight: %g]\n",
	 timing_scan, timing_prepare, total_weight);
  return 0;
}
