/*
 * This file is part of the exercises for the Lectures on 
 *   "Foundations of High Performance Computing"
 * given at 
 *   Master in HPC and 
 *   Master in Data Science and Scientific Computing
 * @ SISSA, ICTP and University of Trieste
 *
 * contact: luca.tornatore@inaf.it
 *
 *     This is free software; you can redistribute it and/or modify
 *     it under the terms of the GNU General Public License as published by
 *     the Free Software Foundation; either version 3 of the License, or
 *     (at your option) any later version.
 *     This code is distributed in the hope that it will be useful,
 *     but WITHOUT ANY WARRANTY; without even the implied warranty of
 *     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *     GNU General Public License for more details.
 *
 *     You should have received a copy of the GNU General Public License 
 *     along with this program.  If not, see <http://www.gnu.org/licenses/>
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "mypapi.h"

#define CYC 0
#define INS 1
#define BRN 2
#define MBR 3
#define ST_CYC 4
							    
#define SIZE_DEFAULT 500000000	 			    /* the default array size           */
#define TOP (2 << 20)					    /* the max int value                */

#define ITER 5						    /* the default # of repetitions     */

							    /* timings                          */
#define TCPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)



#if !defined(BESMART) && !defined(BESMART2) && !defined(BESMART3)
#warning "this code will not be smart"
#define NOSMART
#endif


int main(int argc, char **argv)
{
  
  uint SIZE = SIZE_DEFAULT;
  int  use_random = 0;
  int  iter = ITER;
  int * restrict A, * restrict B;		            /* these hold the original values   */
  int * restrict Ac, * restrict Bc;			    /* these hold the swapped value     */

  struct timespec ts;					    /* used by timing macro             */
  double time_avg = 0, time_std_dev = 0;		    /* used to get the timing estimate  */


							    /* process the aerguments           */
  if(argc > 1)
    {
      use_random = atoi( *(argv+1) );      
      if ( argc > 2 )
	SIZE = atoi( *(argv+2) );	
      if ( argc > 3 )
	iter = atoi( *(argv+3) );

							    /* check the args correctness       */
      if ( SIZE <= 0 )
	SIZE = SIZE_DEFAULT;      
      if ( use_random < 0 || use_random > 1 )
	use_random = 0;
      if ( iter <= 0 )
	iter = ITER;	      
    }

  printf("allocate room for 4x %u elements and initialize it..\n", SIZE);
  
							    /* allocate memory for data         */
  if ( (Ac = (int*)calloc(SIZE*4, sizeof(int))) == NULL )
    {
      printf ( "not enough memory (%llu GB) to allocate %u elements\n",
	       (unsigned long long int)SIZE*4*sizeof(int), SIZE);
      exit( 1 );
    }
							    /* define the pointers              */
  Bc = Ac + SIZE;
  A  = Ac + SIZE*2;
  B  = Ac + SIZE*3;


  if ( use_random )
    {							    
      srand((int)(SIZE));				    /* init the random generator        */

      printf( "%d random numbers generation.. ", SIZE*2 ); fflush(stdout);
      for (uint cc = 0; cc < SIZE*2; cc++)
	Ac[cc] = rand() % TOP;
    }

  else
    {
      printf( "%d predictable numbers generation.. ", SIZE*2 ); fflush(stdout);
      for (uint cc = 0; cc < SIZE/2; cc++)
	Ac[cc] = 1;
      for (uint cc = SIZE/2; cc < SIZE; cc++)
	Ac[cc] = 2;
      
      for (uint cc = 0; cc < SIZE/2; cc++)
	Bc[cc] = 2;
      for (uint cc = SIZE/2; cc < SIZE; cc++)
	Bc[cc] = 1;      
    }
  
  printf( " done\n");

  PAPI_INIT;
  
							    /* initialize the workng arrays     */
  memcpy(A, Ac, SIZE*2*sizeof(int));	


  
  //================================================================================================
							    /* --------------------------------- 
							       : outer loop                    : 
							       : the experiment is iterated    :
							       : iter times to have averaged   :
							       : quantities                    :
							       --------------------------------- */
  for (int cc = 0; cc < iter; cc++)
      {
	double tstart = TCPU_TIME;
	PAPI_FLUSH_BUFFER;
	PAPI_START_CNTR;

							    /* --------------------------------- 
							       : inner loop                    : 
							       :                               :
		                			       : smart, smart2 and smart3 impl.:
	                				       : try to generate either a      :
							       : continuous workflow or a      :
							       : cmov-friendly conditions      :
							       --------------------------------- */

        for (uint ii = 0; ii < SIZE; ii++)
          {
							    /* --------------------------------- */	    
#ifdef NOSMART                                         	    /* the normal implementation         */
							    /* this accounts on the ability of  _*/
            if ( B[ii] > A[ii] )			    /* the CPU's branch predictor        */
	      {
		int t = A[ii];
		A[ii] = B[ii];
		B[ii] = t;
							    /* a different implementation of     */
							    /* two integer swapping              */
							    /*
		A[ii] = A[ii]^B[ii];
		B[ii] = A[ii]^B[ii];
		A[ii] = A[ii]^B[ii];
							    */
	      }
	    
							    /* --------------------------------- */
#elif defined(BESMART)					    /* the smart implementation          */
							    /* this tries to avoid jump instr    */
	                                                    /* adapted from                      */
				                            /* "comp- systems: a programmer's perspective */
	    /* int max = (A[ii] > B[ii]) ? A[ii] : B[ii]; */
	    /* int min = (A[ii] > B[ii]) ? B[ii] : A[ii]; */
	    /* A[ii] = max; */
	    /* B[ii] = min; */

 	    int register AA = A[ii];
	    int register BB = B[ii];
	    int max = (AA > BB)? AA : BB;
	    int min = (AA > BB)? BB : AA;
	    A[ii] = max;
	    B[ii] = min;

							    /* --------------------------------- */	    
#elif defined(BESMART2)					    /* the second smart implementation    */
 			                                    /* adapted from "bit twiddling hacks" */
	    int register AA = A[ii];
	    int register BB = B[ii];
	    int register t = -( AA < BB );
	    int register x = AA^BB;
	    A[ii] = AA^(x & t);   
	    B[ii] = BB^(x & t);
				                            /* what if we write it in a slightly  */
				                            /* differnet way?                     */
				                            /* 
	    int register t = -( A[ii] < B[ii] );
	    int register x = A[ii]^B[ii];
	    A[ii] = A[ii]^(x & t);   
	    B[ii] = B[ii]^(x & t);	    
	                                                    */
				                            /* do you expect it to be more or less*/
				                            /* efficient ?                        */

							    /* --------------------------------- */
#elif defined(BESMART3)					    /* the third smart implementation      */
							    /* this tries to avoid also the ternary_*/
	                        		            /* operator                            */
	    int register AA = A[ii];
	    int register BB = B[ii];
	    int register d = AA - BB;
	    d &= (d >> 31);
	    A[ii] = AA - d; 
	    B[ii] = BB + d;

							    /* pretty equivalent */
	    /* int d = A[ii]-B[ii];		            
	    /* int t = d & (d >> 31); */
	    /* A[ii] = A[ii] - t;  */
	    /* B[ii] = B[ii] + t; */


							    /* this version is not as efficient   */
	    /* int register t = ((A[ii]-B[ii])>>31); */
	    /* int register nt = ~t;                 */
	    /* A[ii] = (t & B[ii]) || (nt & A[ii]);  */
	    /* B[ii] = (nt & B[ii]) || (t & A[ii]);  */	    
#endif
          }                                                  /*---------------------------------
							       : END of inner loop             :
							       ---------------------------------  */
	
	
	double tstop = TCPU_TIME - tstart;		    /* measure timing                      */
	PAPI_STOP_CNTR; 				    /* stop the counters and               */
	                				    /* accumulate the counters values      */

	time_avg     += tstop;				    /* accumulate timing values            */
	time_std_dev += tstop*tstop;

#ifdef DEBUG						    
	if ( cc == 0 )	                                    /* check that actually A[i] > B[i]     */
	  {						    /* only if the code has been compiled  */
	    uint fail = 0;				    /* with -DDEBUG, and after the         */
	    for (uint jj = 0; jj < SIZE; jj++)		    /* first iteration only                */
	      if ( A[jj] < B[jj] )
		fail++;
	    if ( fail > 0 )
	      printf("** WARNING:: %u failures\n", fail );
	  }
#endif
	
	// restore values
	memcpy( A, Ac, 2*SIZE*sizeof(int) );
	
      }                                                      /*---------------------------------
							       : END of outer loop             :
							       ---------------------------------   */

  //================================================================================================



  
  free(Ac);						    /* release memory                      */

							    /* process the timing infos            */
  time_avg    /= iter;
  time_std_dev = sqrt(time_std_dev/iter - time_avg*time_avg);
  printf("\nseconds          : %7.4g +- %6.4g\n", time_avg, time_std_dev);



  
#ifdef USE_PAPI
							    /* process the counters' infos         */
  long double papi_avg[PAPI_EVENTS_NUM];
  for ( int cc = 0; cc < PAPI_EVENTS_NUM; cc++ )
    {
      papi_avg[cc] = ((long double)PAPI_GET_CNTR(cc)/ SIZE) / iter;
      papi_stddev_values[cc] = sqrt(((papi_stddev_values[cc] / SIZE) / SIZE) / iter  - papi_avg[cc]*papi_avg[cc]);
    }


							    /* output useful numbers               */
  
  printf("CPE              : %6s%7.4Lg +- %5.3Lg\n"         /* cycles per element (of the array)   */
	 "IPE              : %13.9Lg +- %7.4Lg\n"	    /* instructions per element            */
	 "BRANCHES_PE      : %13.9Lg +- %7.4Lg\n"	    /* conditional branches per element    */
	 "MISP_BRANCHES_PE : %13.9Lg +- %7.4Lg "	    /* mispredicted cond. brn. per element */
	 " ( tot: %13.9Lg )\n",				    /* total mispredicted cond. branches   */
	 "", papi_avg[CYC], papi_stddev_values[CYC],
	 papi_avg[INS], papi_stddev_values[INS],
	 papi_avg[BRN], papi_stddev_values[BRN],
	 papi_avg[MBR], papi_stddev_values[MBR],
	 papi_avg[MBR]*SIZE);
#endif

  
  printf("\n");

  return 0;
}
