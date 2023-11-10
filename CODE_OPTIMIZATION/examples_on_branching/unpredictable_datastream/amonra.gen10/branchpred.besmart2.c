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



int main(int argc, char **argv)
{
  int  SIZE;
  int *data;
  int  cc, ii;

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


  tstart = TCPU_TIME;
  
  for (cc = 0; cc < 1000; cc++)
      {
	sum = 0;
	long long _sum_[4] = {0};
        for (ii = 0; ii < SIZE; ii+=4)
          {
	    _sum_[0] += (data[ii]>PIVOT? data[ii] : 0);
	    _sum_[1] += (data[ii+1]>PIVOT? data[ii+1] : 0);
            _sum_[2] += (data[ii+2]>PIVOT? data[ii+2] : 0);
            _sum_[3] += (data[ii+3]>PIVOT? data[ii+3] : 0);
          }
	sum += (_sum_[0] + _sum_[1]) + (_sum_[2] + _sum_[3]);
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
