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


#define SIZE_DEFAULT 1000000
#define TOP (2 << 20)
#define PIVOT (TOP >> 2)


#define TCPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)


#ifdef WOW  
int compare(const void *A, const void *B)
{
  return *(int*)A - *(int*)B;
}
#endif

int main(int argc, char **argv)
{
  int  SIZE;
  int *data;
  int  cc, ii;

#ifdef WOW
  double tot_tstart, tot_tstop;
#endif

  long long sum = 0;

  struct timespec ts;
  double tstart, tstop;
  
  if(argc > 1)
    SIZE = atoi( *(argv+1) );
  else
    SIZE = SIZE_DEFAULT;

  // Generate data
  data = (int*)calloc(SIZE, sizeof(int));
  srand((int)(SIZE));
  
  for (cc = 0; cc < SIZE; cc++)
    data[cc] = rand() % TOP;

   
  
#ifdef WOW
  tot_tstart = TCPU_TIME;
  // !!! With this, the next loop runs faster
  qsort(data, SIZE, sizeof(int), compare);
#endif
  

  tstart = TCPU_TIME;
  
  for (cc = 0; cc < 1000; cc++)
      {
	sum = 0;

        for (ii = 0; ii < SIZE; ii++)
          {
#if !defined( BESMART ) && !defined( BESMART2 )
            if (data[ii] > PIVOT)
              sum += data[ii];

#elif defined( BESMART )
            unsigned int t = (data[ii] - PIVOT - 1) >> 31;   // the additional -1 is for the case data[ii]==PIVOT
            sum += ~t & data[ii];

#elif defined( BESMART2 )
	    //sum += (data[ii]>PIVOT)*data[ii];
	    sum += (data[ii]>PIVOT? data[ii] : 0);
#endif
          }
      }

  tstop = TCPU_TIME;
  
#ifdef WOW
  tot_tstop = TCPU_TIME;
#endif
  
  free(data);

 #if !defined(WOW)
  printf("\nsum is %llu, elapsed seconds: %g\n", sum, tstop - tstart);

#else
  double tot_time  = tot_tstop - tot_tstart;
  double loop_time = tstop - tstart;
  printf("\nsum is %llu, elapsed seconds: %g, %g in loop and %g in qsort\n",
	 sum, tot_time, loop_time, tot_time - loop_time);
#endif

  printf("\n");
  return 0;
}
