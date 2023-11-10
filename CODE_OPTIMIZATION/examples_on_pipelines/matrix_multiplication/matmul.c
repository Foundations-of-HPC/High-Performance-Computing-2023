


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


#define _XOPEN_SOURCE 700
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <time.h>
#include "mypapi.h"

#define TCPU (clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +\
	      (double)ts.tv_nsec * 1e-9)

#define NVERSIONS               3
#define SELECT_mask             (1+(1<<1))
#define COMPARE_WITH_PLAIN_mask (1<<2)
#define CHECK_OCCURRENCES_mask  (1<<3)
#define ALL_mask                (~((1<<4)-1))


char *version_labels[NVERSIONS] = {"NON-optimized", "Optimized", "Tailed" };

#define HELP_MESSAGE "\n"						\
  " Calculates C(n,o) = A(n,m) * B(m,o) \n"				\
  " Usage: %s algorithm n m o < block_sizes for A_rows, A_columns, B_columns >\n\n" \
  " implementation:\n"							\
  "      0  - naive\n"							\
  "      1  - optimized\n"						\
  "      2  - block-optimized\n"					\
  " n = A's # of rows, m = A's # of columns, o = C's # of columns\n\n"


void init_matrix(double* M, int n, int m, double v)
{
  for ( int i = 0; i < n; i++)
    {
      int off = i*m + v;
      
      for ( int j = 0; j < m; j++)
	M[off + j ] = off + j;
    }
  return;
}

void clean_matrix(double* M, int n, int m)
{
  memset( M, 0, n*m * sizeof(double));
  return;
}


unsigned int compare_matrices( double *A, double *B, int R, int C, double tolerance )
/*
 * compare matrices A and B, that have R rows and C columns
 * with tolerance
 */
{

  unsigned int errors = 0;
  for ( int i = 0; i < R; i++ )
    {
      int off = i*C;
      
      for ( int j = 0; j < C; j++ )
	{
	  double d = (A[off+j] != 0.0 ? A[off+j] : B[off+j]);
	  d = (d != 0.0) ? d : 1.0;
	  double ratio = (A[off+j] - B[off+j]) / d;
	  ratio = (ratio > 0 ? ratio : -ratio);    // this is to avoi using fabs() from math library
	  if ( ratio > tolerance)
	    errors++;
	}
    }

  return errors;
}

unsigned int check_matrix( double *M, int R, int C, double tolerance) //, int Bc, double vA, double vB, double tolerance)
{

  /*
   * this checks that every entry in the matrix M has the correct value C
   *   
   * In fact, this check should be used with the flag CHECK_OCCURRENCES_mask
   * In that case the product matrix is not set to the correct values:
   * every entry is set to the number of times it had received a contribution,
   * which, by construction, should be exactly C
   * 
   * This is to check that actually the tiling is done properly and, at least
   * each entry of the product matrix receives the right number of contributions
   */
  
  double       value  = (double)C;
  unsigned int errors = 0;
  for ( int i = 0; i < R; i++ ) {
    int off = i*C;
    for ( int j = 0; j < C; j++ ) {
      double ratio = (M[ off + j ] - value) / value;
      ratio = (ratio > 0 ? ratio: -ratio);
      errors += ( ratio > tolerance ); }}


  /*
   * d.o  n.o.t  u.s.e
   * not yet working
   * 
   */  

  /* double _Ac          = (double)Ac; */
  /* double _Bc          = (double)Bc; */
  /* double AcBc         = _Ac * _Bc; */
  /* double sum          = (double)( Ac*(Ac-1)/2 ); */
  /* int    t            = Ac-1;   */
  /* double Bc_sum2      = _Bc * (double)( t + t*t*3 + t*t*t*2 ) / 6.0; */
  

  
  /* for ( int i = 0; i < Ar; i++ ) */
  /*   { */
  /*     int     iA    = i*Ac; */
  /*     double  iAcBc = i * AcBc; */

  /*     for ( int j = 0; j < Bc; j++ ) */
  /* 	{ */
  /* 	  double _j = (double)j; */

  /* 	  double res   = sum*(iAcBc + _j + 2*i*_j) + Bc_sum2; */

  /* 	  double ratio = (M[ iA + j ] - res) / res; */
  /* 	  ratio = (ratio > 0 ? ratio: -ratio); */
  /* 	  if( ratio > tolerance ) */
  /* 	    errors++; */
  /* 	} */
  /*   } */
	 
  return errors;	
}

void straightforward(double* A, double* B, double* C, int Ar, int Ac, int Bc)
/*
 * implements schoolbook's matrix multiplication directly from
 * schoolbook's definition
 *   C_ik = SUM_j Aij * Bjk
 *
 * arguments:
 * A, B, C : matrices
 * Ar      : A's #rows
 * Ac      : A's #columns = B's #rows
 * Bc      : B's #columns
 *
 * note - C = Ar x Bc
 */
{
  PAPI_START_CNTR;
  
  for ( int i = 0; i < Ar; i++ )
    // traverse the A's rows
    //
    {
      int iA_off = i * Ac;
      int iC_off = i * Bc;
      
      for ( int k = 0; k < Bc; k++ )
	// traverse the B's columns
	//
	
	for ( int j = 0; j < Ac; j++ )
	  // multiply the i-th rows in A
	  // per the k-th column in B,
	  // accumulating the result in C_ij
	  //
	  C[iC_off + k] += A[iA_off + j] * B[j*Bc + k];
    }

  PAPI_STOP_CNTR;
}



void straightforward_opt(double* A, double* B, double* C, int Ar, int Ac, int Bc)
/*
 * implements schoolbook's matrix multiplication directly from
 * schoolbook's definition
 *   C_ik = SUM_j Aij * Bjk
 *
 * however, in this version the two innermost loops are swapped
 * in order to decrease the number of L1d misses
 *
 * arguments:
 * A, B, C : matrices
 * Ar      : A's #rows
 * Ac      : A's #columns = B's #rows
 * Bc      : B's #columns
 *
 * note - C = Ar x Bc
 */
{
  PAPI_START_CNTR;
  
  for ( int i = 0; i < Ar; i++ )
    // traverse the A's rows
    {
      int iA_off = i * Ac;
      int iC_off = i * Bc;

      for ( int j = 0; j < Ac; j++ )
	{
	  int jB_off = j*Bc;
	
	  for ( int k = 0; k < Bc; k++ )
	    // the dominant L1d misses were
	    // due to the strided access to B,
	    // which happens
	    //    Ar x Ac x Bc
	    // times. In this way, B's elements
	    // are traversed subsequently, instead
	    // 
	    C[iC_off + k] += A[iA_off + j] * B[jB_off + k];
	}
    }

  PAPI_STOP_CNTR;
}



void straightforward_opt_blocks ( double * restrict A, double * restrict B, double * restrict C,
				  int Ar, int Ac, int Bc,
				  int Arb, int Acb, int Bcb )
  
/*
 * implements schoolbook's matrix multiplication directly from
 * schoolbook's definition
 *   C_ik = SUM_j Aij * Bjk
 *
 * however, in this version the two innermost loops are swapped
 * in order to decrease the number of L1d misses
 *
 * arguments:
 * A, B, C : matrices
 * Ar      : A's #rows
 * Ac      : A's #columns = B's #rows
 * Bc      : B's #columns
 * Arb     : block size for A's row
 * Acb     : block size for A's columns & B's row
 * Bcb     : block size for B's columns
 *
 * note - C = Ar x Bc
 */
{
  PAPI_START_CNTR;
  
  int Ar_N = Ar / Arb + Ar % Arb;
  int Ac_N = Ac / Acb + Ac % Acb;
  int Bc_N = Bc / Bcb + Bc % Bcb;
  
  for ( int ii = 0; ii < Ar_N; ii++ )
    {
      int i_start = ii*Arb;
      int i_end   = i_start + Arb;
      i_end       = (i_end <= Ar ? i_end : Ar);

      for ( int jj = 0; jj < Ac_N; jj++ )
	{
	  int j_start = jj*Acb;
	  int j_end   = j_start + Acb;
	  j_end       = (j_end <= Ac ? j_end : Ac);
	  
	  for ( int i = i_start; i < i_end; i++ )
	    // run over the the A's rows
	    //
	    {
	      int          iA_off = i*Ac;
	      int register iC_off = i*Bc;

	      for ( int j = j_start; j < j_end; j++)
		// run over A's columns and B's rows
		//
		{
		  int register j_off = j*Bc;
		  int register ij_off = iA_off + j;

		  for ( int kk = 0; kk < Bc_N; kk++ )
		    {
		      int k_start = kk*Bcb;
		      int k_end   = k_start + Bcb;
		      k_end       = (k_end <= Bc? k_end : Bc);
		      
		      for ( int k = k_start; k < k_end; k++)
			// run over B's and C's columns
			//
			//C[iC_off + k] += 1.0; // to be used with check_matrix
			                        // instead of the next line
			C[iC_off + k] += A[ij_off] * B[j_off + k];
		    }
		}
	    }
	}
    }

  PAPI_STOP_CNTR;
}





int main(int argc, char** argv)
{

  // check whether there the
  // minimum number of parameters
  // is present
  //
  
   if (argc < 5)
     {
       printf( HELP_MESSAGE, argv[0] );
       /*
       printf(" Calculates C(n,o) = A(n,m) * B(m,o) \n");
       printf(" Usage: %s algorithm n m o < block_sizes for A_rows, A_columns, B_columns >\n"
	      "   implementation:\n"
	      "      0  - naive\n"
	      "      1  - optimized\n"
	      "      2  - block-optimized\n"
	      "   n = A's # of rows, m = A's # of columns, o = C's # of columns\n", argv[0]);
       */
       return 1;
     }

   // get the basic arguments
   //
   
   int w  = atoi(argv[1]);  // what implementation to use   
   int Ar = atoi(argv[2]);  // A's # of rows
   int Ac = atoi(argv[3]);  // A's # of columns = B's # of rows
   int Bc = atoi(argv[4]);  // B's # of columns

   // set / get parameters for blocking implementation
   //
   int Arb, Acb, Bcb;
   if ( w & 2 )
     {
       if ( argc > 5 ) { if (argc < 8 ) { printf( HELP_MESSAGE, argv[0] ); return 2; };
	 Arb = atoi(argv[5]);  // A's # of rows
	 Acb = atoi(argv[6]);  // A's # of columns = B's # of rows
	 Bcb = atoi(argv[7]); }// B's # of columns       
       else
	 Arb = Acb = Bcb = ( Ac > 128 ? 128 : Ac/2);
       printf("using block size: %d %d %d\n", Arb, Acb,Bcb);
     }

   // print memory occupation
   // 
   unsigned long long int allocation_size = ((Ar * Ac) + (Ac * Bc) + (Ar * Bc));
   if ( w & COMPARE_WITH_PLAIN_mask )
     // if we are requesting to verify the results,
     // we need some additional memory
     //
     allocation_size += Ar * Bc;
   printf("memory needed: %llu doubles, %7.4g GB\n",
	  allocation_size,
	  (double)(allocation_size*sizeof(double)) / (1024*1024*1024));


   // allocate memory
   //
   double *A = malloc(Ar * Ac * sizeof(double));
   double *B = malloc(Ac * Bc * sizeof(double));
   double *C = malloc(Ar * Bc * sizeof(double));

   // initialize matrices
   //
   init_matrix (A, Ar, Ac, 0);
   init_matrix (B, Ac, Bc, 0);  
   clean_matrix(C, Ar, Bc);
   
   // some useful variables
   //
   struct timespec  ts;
   int              version = w & SELECT_mask;
   double           tbegin, tstop;

   PAPI_INIT;
   
   switch( version )     // mask the 2 least significant bits
                         // so to check the implementation to run
     {
     case 0:
       tbegin = TCPU;
       straightforward(A, B, C, Ar, Ac, Bc);
       tstop   = TCPU - tbegin;
       break;
       
     case 1:       
       tbegin = TCPU;
       straightforward_opt(A, B, C, Ar, Ac, Bc);
       tstop   = TCPU;
       break;

     case 2:
       tbegin = TCPU;
       straightforward_opt_blocks(A, B, C, Ar, Ac, Bc, Arb, Acb, Bcb);
       tstop   = TCPU;
       break;
       
     default:
       if ( w & ALL_mask )
	 printf("unknown request number: %d\n", w);       
     }

   printf( "elapsed time: %9.4f s for version: %-20s\n\n", tstop, (version < NVERSIONS? version_labels[w & SELECT_mask] : ""));

   if ( w & COMPARE_WITH_PLAIN_mask )
     {
       double *D = malloc(Ar * Bc * sizeof(double));
       printf("calculating AxB with the naive implementation..");
       straightforward(A, B, D, Ar, Ac, Bc);
       printf("done\n"
	      "checking the result against plain implementation.. ");
       unsigned int errors_check = compare_matrices( C, D, Ar, Bc, 1e-4);
       printf("%u errors detected\n", errors_check);
       free(D);
     }
   
   if ( w & CHECK_OCCURRENCES_mask )
     {
       printf("checking the # of times each entries has been updated.. ");
       unsigned int errors_check = check_matrix( C, Ar, Bc, 1e-4);
       printf("%u errors detected\n", errors_check);
     }
   
   free(A);
   free(B); 
   free(C);

   printf("done\n");
   
  #if defined(USE_PAPI)
   uLint N3 = (uLint)Ar * (uLint)Ac * (uLint)Bc;
   printf( "%25s: %-4.2g\n"
	   "%25s: %-6.4g\n"
	   "%25s: %-6.4g\n"
	   "%25s: %-6.4g\n",
	   
	   "IPC",
	   (double)papi_values[0] / papi_values[1],
	   
	   "time-per-element (nsec)",
	   tstop / N3 * 1e9,

	   "cycles-per-element",
	   (double)papi_values[1] / N3,

	   "L1miss-per-element",
	   (double)papi_values[2] / N3);

  #endif
   
   return 0;

}
