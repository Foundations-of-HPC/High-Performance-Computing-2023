

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
#include <math.h>

#include "reduction.h"
#include "mypapi.h"

#define N_default  1000000
#define simple     0 
#define unroll2x1  1
#define unroll2x1g 2
#define unroll2x2  3
#define unroll4x2g 4
#define unroll4x4  5
#define vunroll4x4 6
#define runroll2x2 7
#define unroll8x4  8

#define ITER       10
#define ALIGN      32

#if !defined(OP)
#define OP +
#endif

DATYPE scan            ( const uLint, DTYPE * restrict );
DATYPE scan_unroll2x1  ( const uLint, DTYPE * restrict );
DATYPE scan_unroll2x1g ( const uLint, DTYPE * restrict );
DATYPE scan_unroll2x2  ( const uLint, DTYPE * restrict );
DATYPE scan_unroll4x2g ( const uLint, DTYPE * restrict );
DATYPE scan_unroll4x4  ( const uLint, DTYPE * restrict );
DATYPE scan_unroll8x4  ( const uLint, DTYPE * restrict );
DATYPE vscan_unroll4x4 ( const uLint, DTYPE * restrict );



DATYPE scan( const uLint N, DTYPE * restrict array )
{

  DATYPE acc = 0;
  for ( uLint ii = 1; ii < N; ii++ )
    acc = acc OP array[ii];
  
  return acc;
}

DATYPE scan_unroll2x1( const uLint N, DTYPE * restrict array )
{

  uLint    N_2  = (N/2)*2;

  DATYPE acc = 0;
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 0; ii < N_2; ii+=2 )
    {
      acc = (acc OP array[ii]) OP array[ii+1];
    }

  for ( uLint ii = N_2; ii < N; ii++ )
    acc = acc OP array[ii];

  return acc;
}


DATYPE scan_unroll2x1g( const uLint N, DTYPE * restrict array )
{

  uLint    N_2  = (N/2)*2;

  DATYPE acc = 0;
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 0; ii < N_2; ii+=2 )
    {
      acc = acc OP (array[ii] OP array[ii+1]);
    }

  for ( uLint ii = N_2; ii < N; ii++ )
    acc = acc OP array[ii];

  return acc;
}


DATYPE scan_unroll2x2( const uLint N, DTYPE * restrict array )
{

  uLint    N_2  = (N/2)*2;

  DATYPE acc[2] = {0};
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 0; ii < N_2; ii+=2 )
    {
      acc[0] = acc[0] OP array[ii];
      acc[1] = acc[1] OP array[ii+1];
    }

  for ( uLint ii = N_2; ii < N; ii++ )
    acc[0] = acc[0] OP array[ii];
  acc[0] = acc[0] OP acc[1];
  
  return acc[0];
}


DATYPE rscan_unroll2x2( const uLint N, DTYPE * restrict array )
// we should find that there is no difference with
// scan_unroll2x2
//
{

  uLint    N_2  = (N/2)*2;

  DATYPE register acc0 = 0;
  DATYPE register acc1 = 0;
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 0; ii < N_2; ii+=2 )
    {
      acc0 = acc0 OP array[ii];
      acc1 = acc1 OP array[ii+1];
    }

  for ( uLint ii = N_2; ii < N; ii++ )
    acc0 = acc0 OP array[ii];
  acc0 = acc0 OP acc1;
  
  return acc0;
}


DATYPE scan_unroll4x2g( const uLint N, DTYPE * restrict array )
{

  uLint    N_4  = (N/4)*4;

  DATYPE acc[2] = {0};
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 0; ii < N_4; ii+=4 )
    {
      acc[0] = acc[0] OP (array[ii] OP array[ii+1]);
      acc[1] = acc[1] OP (array[ii+2] OP array[ii+3]);
    }

  for ( uLint ii = N_4; ii < N; ii++ )
    acc[0] = acc[0] OP array[ii];
  acc[0] = acc[0] OP acc[1];
  
  return acc[0];
}


DATYPE scan_unroll4x4( const uLint N, DTYPE * restrict array )
{

  uLint    N_4  = (N/4)*4;

  DATYPE register acc[4] = {0};
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 0; ii < N_4; ii+=4 )
    {
      acc[0] = acc[0] OP array[ii];
      acc[1] = acc[1] OP array[ii+1];
      acc[2] = acc[2] OP array[ii+2];
      acc[3] = acc[3] OP array[ii+3];
    }

  acc[1] = acc[1] OP (acc[2] OP acc[3]);
  for ( uLint ii = N_4; ii < N; ii++ )
    acc[0] = acc[0] OP array[ii];
  acc[0] = acc[0] OP acc[1];
  
  return acc[0];
}



DATYPE scan_unroll8x4( const uLint N, DTYPE * restrict array )
{
  #define STRIDE 8
  uLint    N_  = (N/STRIDE)*STRIDE;

  DATYPE register acc0[STRIDE/2] = {0};
  DATYPE register acc1[STRIDE/2] = {0};
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 0; ii < N_; ii+=STRIDE )
    {
      acc0[0] = array[ii];
      acc0[1] = array[ii+1];
      acc0[2] = array[ii+2];
      acc0[3] = array[ii+3];
      
      acc1[0] = array[ii+STRIDE];
      acc1[1] = array[ii+STRIDE+1];
      acc1[2] = array[ii+STRIDE+2];
      acc1[3] = array[ii+STRIDE+3];

      acc0[0] = acc0[0] OP acc1[0];
      acc0[0] = acc0[1] OP acc1[1];
      acc0[0] = acc0[2] OP acc1[2];
      acc0[0] = acc0[3] OP acc1[3];
    }

  acc0[1] = acc0[1] OP (acc0[2] OP acc0[3]);
  for ( uLint ii = N_; ii < N; ii++ )
    acc0[0] = acc0[0] OP array[ii];
  acc0[0] = acc0[0] OP acc0[1];
  
  return acc0[0];
}



DATYPE vscan_unroll4x4( const uLint N, DTYPE * restrict array )
{

  uLint    N_4  = N/4;

  v4d *varray = __builtin_assume_aligned(array, ALIGN);
  u4d sum;
  
  PRAGMA_VECT_LOOP
  for ( uLint register ii = 0; ii < N_4; ii++ )
    sum.v = sum.v OP varray[ii];

  sum.s[0] = sum.s[0] OP sum.s[1];
  sum.s[2] = sum.s[2] OP sum.s[3];
  
  DATYPE acc = 0;
  for ( uLint ii = (N/4)*4; ii < N; ii++ )
    acc = acc OP array[ii];

  acc = acc OP (sum.s[0] OP sum.s[2]);
  
  return acc;
}



int main ( int argc, char **argv )
{
  
  struct timespec ts;  
  int             Nth_level1 = 1;
  int             Nth_level2 = 0;
  
  // -------------------------------------------------------------
  // variables' initialization to default values
  //

  int    N         = N_default;
  int    scan_type = simple;
  
  
  if ( argc > 1 )
    {
      scan_type = atoi( *(argv+1) );
      if ( argc > 2 )
	N  = atoi( *(argv+2) );
    }

  printf( "scan type: %d\n", scan_type );

  
  // -------------------------------------------------------------
  // data init.

  double timing_array[ITER+1] = {0};
  double total_weight;
  
  int    N_alloc = ((N/4)+1)*4;
  DTYPE *array   = (DTYPE*)aligned_alloc( ALIGN, N_alloc * sizeof(DTYPE) );

  PAPI_INIT;

  double timing_start = CPU_TIME; 

  for ( int ii = 0; ii < N; ii++ )
    array[ii] = (DTYPE)ii;

  double timing_prepare = CPU_TIME;
  
  for(int k = 0; k < ITER; k++)
    {
     
      double time_go = CPU_TIME;
      PAPI_START_CNTR;

      switch( scan_type )
	{
	  
	case simple:	
	  total_weight = scan( N, array ); break;	 

	case unroll2x1:
	  total_weight = scan_unroll2x1( N, array ); break;

	case unroll2x1g:
	  total_weight = scan_unroll2x1g( N, array ); break;

	case unroll2x2:	 
	  total_weight = scan_unroll2x2( N, array ); break;

	case unroll4x2g:
	  total_weight = scan_unroll4x2g( N, array ); break;
	  
	case unroll4x4:	
	  total_weight = scan_unroll4x4( N, array ); break;
	  
	case vunroll4x4:	
	  total_weight = vscan_unroll4x4( N, array ); break;

	case runroll2x2:	 
	  total_weight = rscan_unroll2x2( N, array ); break;

	case unroll8x4:
	  total_weight = scan_unroll8x4( N, array ); break;
	  
	default:
	  if( k == 0)
	    printf("  >>>> unknown option, no run will be executed\n");
	}
      
      PAPI_STOP_CNTR;
      double deltat = CPU_TIME-time_go;

      timing_array[ ITER ] = deltat;
      {
	int p = ITER;
	while( (p > 0) &&
	       ( (timing_array[p-1] == 0) ||
		 (timing_array[p-1] > deltat) ) )
	  {
	    double _t_ = timing_array[p-1];
	    timing_array[p-1] = deltat;
	    timing_array[p] = _t_;
	    p--;
	  }
      }
	
      //timing_scan        += deltat;
      //timing_scan_stddev += deltat*deltat;

    }

  double timing_stddev = 0;
  double timing_avg = 0;

  {
    int max_t = (ITER > 5 ? 5 : ITER);
    for ( int t = 0; t < max_t; t++ )
      timing_avg += timing_array[t], timing_stddev += timing_array[t]*timing_array[t];
    timing_avg /= max_t;
    timing_stddev = sqrt( timing_stddev/max_t - timing_avg*timing_avg);
  }
  
  printf("timing for scan is %g +- %g    timing for prepare is %g [total weight: %g]\n",
	 timing_avg, timing_stddev, timing_prepare-timing_start, total_weight);

 #if defined(USE_PAPI)
  double _Ninv = (double)1.0/(ITER*N);
  printf( "%25s: %-4.2g\n"
	  "%25s: %-6.4g\n"
	  "%25s: %-6.4g\n"
	  "%25s: %-6.4g\n",
	  
	  "IPC",
	  (double)papi_values[0] / papi_values[1],
	  
	  "time-per-element (nsec)",
	  timing_avg / N,
	  
	  "cycles-per-element",
	  (double)papi_values[1] * _Ninv,
	  
	  "L1miss-per-element",
	  (double)papi_values[2] * _Ninv);
  
 #endif


  return 0;
}
