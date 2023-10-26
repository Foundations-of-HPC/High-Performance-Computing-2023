
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
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <omp.h>



int main( int argc, char **argv )
{

  long double PI              = 3.141592653589;
  int         morning_coffees = 3;
  char        passwd[]        = "DontAsk_DontTell";
  int         passwd_len      = strlen(passwd);
  int         final_mark;
  int         Niter;
  srand( time(NULL) );
  Niter = rand() % 100;

  printf("Niter: %d\n", Niter);
  // usual exercise: inspect the address of the variables
  printf("\nThe addresses of shared-memory variables are as follows:\n"
	 "PI              : %12p [value is %12.9Lg]\n"
	 "morning coffees : %12p [value is %12d]\n"
	 "passwd          : %12p [value is %12s] (len is %td)\n"
	 "final_mark      : %12p [value is %12d]\n\n",
	 &PI, PI,
	 &morning_coffees, morning_coffees,
	 &passwd, passwd, strlen(passwd),
	 &final_mark, final_mark );

#pragma omp parallel firstprivate(PI, morning_coffees) private(passwd) shared(Niter)
  {
    // usual exercise: inspect the address of the variables

    int myid = omp_get_thread_num();
    int nthreads = omp_get_num_threads();
    
    PI              += myid;
    morning_coffees += myid;
    printf("th %d, %td\n", myid, strlen(passwd));
    sprintf( passwd, "p::%-*d", passwd_len-4, myid);
    
    int seed = myid;

    #pragma omp for lastprivate(final_mark)
    for ( int i = 0; i < Niter; i++)
      final_mark = myid*100 + (rand_r(&seed) % 33);

    printf("Thread %d\n\tThe addresses of private variables are as follows:\n"
	   "\tPI              : %12p [value is %12.9Lg]\n"
	   "\tmorning coffees : %12p [value is %12d]\n"
	   "\tpasswd          : %12p [value is %12s] (len is %td)\n"
	   "\tfinal_mark      : %12p [value is %12d]\n\n",
	   myid, &PI, PI,
	   &morning_coffees, morning_coffees,
	   &passwd, passwd, strlen(passwd),
	   &final_mark, final_mark );

  }


  printf("------------------------------------------\n"
	 "The addresses of shared-memory variables are as follows:\n"
	 "PI              : %12p [value is %12.9Lg]\n"
	 "morning coffees : %12p [value is %12d]\n"
	 "passwd          : %12p [value is %12s] (len is %td)\n"
	 "final_mark      : %12p [value is %12d]\n",
	 &PI, PI,
	 &morning_coffees, morning_coffees,
	 &passwd, passwd, strlen(passwd),
	 &final_mark, final_mark );

  return 0;
}
