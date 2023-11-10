
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
#include <math.h>
#include <time.h>

/*
 * define at compile line 
 *   -DIF_FOREST to compile with a lot of if inside the loop;
 *   -DPOINTERS to compile using function pointers inside the loop
 *   -DSWITCH to compile using a switch construct inside the loop
 *
 * if none is defined, the default is IF_FOREST
 */


#define CPU_TIME (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), (double)ts.tv_sec +	\
		   (double)ts.tv_nsec * 1e-9)


void F_000(double *R, int i)
{
  *R += i;
}

void F_001(double *R, int i)
{
  *R -= i;
}

void F_010(double *R, int i)
{
  *R *= i;
}

void F_011(double *R, int i)
{
  *R /= i;
}

void F_100(double *R, int i)
{
  *R += log10((double)i+1);
}

void F_101(double *R, int i)
{
  *R -= log10((double)i+1);
}

void F_110(double *R, int i)
{
  *R *= sin((double)i);
}

void F_111(double *R, int i)
{
  double di = (double)i;
  *R /= (sin(di) + cos(di));
}


#define mode_default  0
#define top_default   20000000
#define case1_default 1
#define case2_default 1
#define case3_default 1


int main(int argc, char **argv)
{
  int mode  = mode_default;
  int top   = top_default;
  int case1 = case1_default;
  int case2 = case2_default;
  int case3 = case3_default;
  
  struct timespec ts;

  if (argc > 1)
    {
      mode  = atoi(*(argv+1));
      if ( argc > 4 )
	{
	  case1 = atoi(*(argv+2));
	  case2 = atoi(*(argv+3));
	  case3 = atoi(*(argv+4));

	  if ( argc > 5 )
	    top   = atoi(*(argv+5));
	}
    }
  

  double result = 1.0;
  int volatile i;

  double start = CPU_TIME;
  
  if( mode == 0 )
    /*
     * IF-FOREST inside the loop
     */
    {
      printf("running if-forest..\n");
      
      for ( i = 1; i < top; i++)
	{
	  if(case1 == 0)
	    {
	      if(case2 == 0)
		{
		  if(case3 == 0)
		    result += i;
		  else
		    result -= i;
		}
	      else
		{
		  if(case3 == 0)
		    result *= i;
		  else
		    result /= i;
		}
	    }
	  else
	    {
	      if(case2 == 0)
		{
		  if(case3 == 0)
		    result += log10((double)i+1);
		  else
		    result -= log10((double)i+1);
		}
	      else
		{
		  if(case3 == 0)
		    result *= sin((double)i);
		  else
		    result /= (sin((double)i) + cos((double)i));
		}
	    }
              
	}
    }

  else if ( mode == 1 )
    /*
     * FUNCTION POINTERS
     */
    {

      printf("running with funciton pointer..\n");
      
      void (*func)(double*, int);
      
      if(case1 == 0)
	{
	  if(case2 == 0)
	    {
	      if(case3 == 0)
		func = &F_000;
	      else
		func = &F_001;
	    }
	  else
	    {
	      if(case3 == 0)
		func = &F_010;
	      else
		func = &F_011;
	    }
	}
      else
	{
	  if(case2 == 0)
	    {
	      if(case3 == 0)
		func = &F_100;
	      else
		func = &F_101;
	    }
	  else
	    {
	      if(case3 == 0)
		func = &F_110;
	      else
		func = &F_111;
	    }
	}
  
      for( i = 1; i < top; i++)
	{
	  double temp;
	  func(&temp, i);
	  result = temp;
	}
    }
  
  else if ( mode == 2 )
    /*
     * SWITCH cases
     */
    {

      printf("running switch..\n");
	    
      int _case_ = (case1 << 2) + (case2 << 1) + case3;

      for ( int i = 0; i < top; i++ )
	{
	  switch( _case_ )
	    {

	      // (0,0,0)
	    case 0: result += i; break;
	  
	      // (0,0,1)
	    case 1: result -= i; break;

	      // (0,1,0)
	    case 2: result *= i; break;

	      // (0,1,1)
	    case 3: result /= i; break;

	      // (1,0,0)
	    case 4: result += log10((double)i+1); break;

	      // (1,0,1)
	    case 5: result -= log10((double)i+1); break;

	      // (1,1,0)
	    case 6: result *= sin((double)i); break;

	      // (1,1,1)
	    case 7: result /= (sin((double)i) + cos((double)i)); break;

	    default:
	      ;
	    }
	}
    }

  
  double stop = CPU_TIME;
  printf("elapsed seconds: %g\n", stop - start);

      
  printf("result is : %g\n", result);

  return 0;
}
