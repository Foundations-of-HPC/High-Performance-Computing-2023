#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <sys/resource.h>
#include <sys/times.h>
#include <time.h>

#include "mypapi.h"

/*
 * -------------------------------------
 *
 *   define types and default values
 *
 * ------------------------------------- 
 */


#if defined( INT8  )                            // use 64bits integers

#warning "using long long integers"
typedef unsigned long long int data_t;

#else                                           // use 32bits integers

typedef unsigned int data_t;
#warning "using integers"

#endif

#define COLUMNS_DFLT 128LLU
#define ROWS_DFLT    128LLU
#define BLOCKSIZE_DFLT 64

typedef unsigned long long int idx_t;

#define CPU_TIME ({struct timespec ts; clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), \
					 (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;})


extern inline void transpose ( int, idx_t, idx_t, data_t **, data_t ** );

inline void transpose ( int mode, idx_t nrows, idx_t ncols, data_t **M, data_t **T )
{
  switch ( mode )
    {
    case 0: {
      for ( idx_t i = 0; i < nrows; i++ )
	for ( idx_t j = 0; j < ncols; j++ )
	  T[j][i] = M[i][j]; }; break;
    case 1: {
      for ( idx_t i = 0; i < nrows; i++ )
	for ( idx_t j = 0; j < ncols; j++ )
	  T[i][j] = M[j][i]; }; break;
    }
  
  return; 
}



/*
 * -------------------------------------
 *
 *   main
 *
 * ------------------------------------- 
 */


int main(int argc, char **argv)
{
  
  int block_size = (argc > 1 ? atoi(*(argv+1)) : BLOCKSIZE_DFLT );   // get the numbr of columns
  int nrows      = (argc > 2 ? atoi(*(argv+2)) : ROWS_DFLT );        // get the number of rows
  int ncols      = (argc > 3 ? atoi(*(argv+3)) : COLUMNS_DFLT );     // get the numbr of columns
  int check      = (argc > 4 ? atoi(*(argv+4)) : 0 );                // about cheking the final matrix

  if ( (nrows < 1) ||
       (ncols < 1) )
    {
      printf( "both nrows and ncols should be larger than 0\n");
      exit(1);
    }
  
  if ( (block_size < 1 ) ||
       (block_size > nrows ) ||
       (block_size > ncols ) )
    {
      printf( "block size should be >=1 and not larger of the minimum btw rows and cols\n" );
      exit(1);
    }
  
  /*
   * allocate the memory
   */
  data_t **matrix  = (data_t**)malloc( (nrows+ncols)*sizeof(data_t*) );    // the original matrix
  data_t **tmatrix = matrix + nrows;                                       // the transposed matrix

  /*
   * set-up the pointers that represent the matrix
   */  
  data_t *all      = (data_t*)calloc( 2*nrows*ncols, sizeof(data_t) );
  for ( idx_t i = 0; i < nrows; i++ )
    matrix[i]  = all + i*ncols;
  
  for ( idx_t i = 0; i < ncols; i++ )
    tmatrix[i] = all + nrows*ncols + i*nrows;
  
  int half_datat = (sizeof(data_t)*4);                               // the number of bits for half
								     // of the data type (that is used
								     // to initialize and check the
								     // matrix entries)

  /*
   * generate the original matrix
   */


  for ( idx_t row = 0; row < nrows; row++ )
    {
      //
      // each entry has the values of the pair
      //   row, column
      // stored in the upper and bottom half
      // respectively
      // In this way it will be easy to check
      // the correctness of the transposed matrix
      //
      
      data_t value = (row << half_datat );
      for ( idx_t col = 0; col < ncols; col++ )
	matrix[row][col] = value | col;
    }
	  
  /*
   * transpose the matrix
   */

  double timing;
  PAPI_INIT;

  if ( block_size == 1 )
    {
      timing = CPU_TIME;
      PAPI_START_CNTR;
      
      for ( idx_t ri = 0; ri < nrows; ri++ )	    	 
	for( idx_t ci = 0; ci < ncols; ci++ )
	  tmatrix [ci][ri] = matrix[ri][ci];

      PAPI_STOP_CNTR;
      timing = CPU_TIME - timing;
    }
  
  else
    {
  
      idx_t row_nblocks = nrows / block_size - (nrows % block_size==0);                         // how many row-blocks there wiil be
      idx_t row_remind  = (nrows % block_size ? nrows - row_nblocks*block_size : block_size);   // account for a final block of
      //   smaller size
      
      idx_t col_nblocks = ncols / block_size - (ncols % block_size==0);                         // how many column-blocks there wiil be
      idx_t col_remind  = (ncols % block_size ? ncols - col_nblocks*block_size : block_size);   // account for a final block of
      //    smaller size
      
      timing = CPU_TIME;
      
      PAPI_START_CNTR;
      // actually transpose the matrix
      //  
      for ( idx_t rb = 0; rb <= row_nblocks; rb++ )
	// loop over the blocks along the rows
	//
	{
	  idx_t row_start = rb*block_size;
	  idx_t row_end   = row_start +( rb < row_nblocks ? block_size : row_remind );
	  
	  for ( idx_t cb = 0; cb <= col_nblocks; cb++ )
	    // loop over the blocks along the columns
	    //
	    {
	      idx_t col_start = cb*block_size;
	      idx_t col_end   = col_start + ( cb < col_nblocks ? block_size : col_remind );
	      
	      // transpose the current block
	      // n.b. strided access is in write
	      for ( idx_t ri = row_start; ri < row_end; ri++ )	    	 
		for( idx_t ci = col_start; ci < col_end; ci++ )
		  tmatrix [ci][ri] = matrix[ri][ci];
	    }      
	}
      PAPI_STOP_CNTR;
      timing = CPU_TIME - timing;
    }
  


  if ( check )
    {
      /*
       * check the transposed matrix
       */

      idx_t faults = 0;
      for ( idx_t col = 0; col < ncols; col++ )
	{
	  data_t value = col;
      
	  for ( idx_t row = 0; row < nrows; row++ )
	    {
	      //
	      // each entry has the values of the pair
	      //   row, column
	      // stored in the upper and bottom half
	      // respectively
	      // In this way it will be easy to check
	      // the correctness of the transposed matrix
	      //

	      faults+= (tmatrix[col][row] != (value | (row<<half_datat)));
	    }
	}

      if( faults )
	printf("%llu faults found!\n", faults);
      else
	printf("the transposed matrix is ok\n");

    }
  
  printf("transpose time is %g\n", timing );

 #if defined(USE_PAPI)
  uLint N = nrows * ncols;
  printf( "%25s: %-4.2g\n"
	  "%25s: %-6.4g\n"
	  "%25s: %-6.4g\n"
	  "%25s: %-6.4g\n",
	  
	  "IPC",
	  (double)papi_values[0] / papi_values[1],
	  
	  "time-per-element (nsec)",
	  timing / N * 1e9,
	  
	  "cycles-per-element",
	  (double)papi_values[1] / N,
	  
	  "L1miss-per-element",
	  (double)papi_values[2] / N);
  
 #endif


  free ( all );
  free ( matrix );
  
  return 0;
}
