
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
#include <omp.h>


int main( int argc, char **argv )
{

  int nthreads;

#if defined(_OPENMP)

  int threads_num = 1;

  if ( argc > 1 )
				             // read the argument given
    threads_num = atoi(*(argv+1));

  else
    {
                                             // if no arguments are given,
                                             // tries to read the env variable
      char *buffer = getenv("OMP_NUM_THREADS");
      if ( buffer != NULL )
	threads_num = atoi( buffer );
      else
	{
                                             // if OMP_NUM_THREADS is not defined,
	                                     // reads the /proc/cpuinfo by opening
	                                     // a pipe with a shell
	  FILE *pipe = popen("cat /proc/cpuinfo | grep processor | cut -d':' -f2 | tail -n1", "r");
	  if ( pipe != NULL )
	    {
                 	                     // reads the pipe output, i.e. the
	                                     // number of cores
	      char   *string_ = NULL;
	      size_t  string_size;
	      if( getline( &string_, &string_size, pipe) > 0 )
		threads_num = atoi( string_ ) + 1;
	      pclose(pipe);
	      if ( string_ != NULL )
		free( string_ );
	    }
 	                                     // if even that fails, just use 1 thread
	}
    }

  if ( omp_get_dynamic( ) == 0 )
    {
      int max_allowed = omp_get_max_threads();
      if ( max_allowed != threads_num )
	printf("\t>>> your OMP_DYNAMIC variable is set to FALSE\n"
	       "\t    your omp could not adapt the number of threads in\n"
	       "\t    a parallel region to optimize the use of the system.\n"
	       "\t    Still, you can adapt the number of threads by hands\n"
	       "\t    in each parallel region by using \"omp_set_num_threads()\"\n"
	       "\t    Currently, the default maximum allowed number of threads\n"
	       "\t    in a PR (without you modifying that) is %d\n", max_allowed );
    }

  omp_set_num_threads( threads_num );

  
#pragma omp parallel               // this creates a parallel region
                                   // that is encompassed by the
                                   // opening and closing { }
                                   //
                                   // you can modify the number of
                                   // spawned threads in different
                                   // ways:
                                   // 1) through the OMP_THREAD_NUM
                                   //    environmental variable
                                   // 2) using the omp_set_num_threads()
                                   //
                                   // you can also declare the desired
                                   // number at the creation of the
                                   // parallel region:
  
  // #pragma omp parallel num_threads( threads_num )
  
  {   
    
    int my_thread_id = omp_get_thread_num();
    #pragma omp master
    nthreads = omp_get_num_threads();

                                   // the order in which different threads will
                                   // arrive at this print is undefined;
                                   // if you run this code several times, you will
                                   // obtain different results

    printf( "\tgreetings from thread num %d\n", my_thread_id );
  }

#else

  nthreads = 1;

#endif

  printf(" %d thread%s greeted you from the %sparallel region\n",
	nthreads, (nthreads==1)?" has":"s have", (nthreads==1)?"(non)":"" );  
  
  return 0;
}
