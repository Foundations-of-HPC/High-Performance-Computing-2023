
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
#if !defined(_OPENMP)
#error "OpenMP support is mandatory for this code"
#endif
#define _GNU_SOURCE
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <omp.h>


int get_some_work_done_on_sockets( int, char*** );

int main( void )
{

  int nthreads           = 1;
  int nthreads_requested = 1;
  
  char *places = getenv("OMP_PLACES");
  char *bind   = getenv("OMP_PROC_BIND");
  if ( places != NULL )
    printf("OMP_PLACES is set to %s\n", places);
  if ( bind != NULL )
    printf("OMP_PROC_BINDING is set to %s\n", bind);

  omp_set_nested(1);
  omp_set_max_active_levels(2);

  // get how many places (they are supposed to be sockets,
  // please define the OMP_PLACES accordingly)
  // are available in the place list
  int nsockets = omp_get_num_places();    

  
 #pragma omp parallel num_threads(nsockets) proc_bind(spread)
  {    
   #pragma omp master
    {
      char *proc_bind_names[] = { "false (no binding)",
				  "true",
				  "master",
				  "close",
				  "spread" };
      
      nthreads = omp_get_num_threads();
      
      // get the current binding
      int binding = omp_get_proc_bind();

      printf("+ %d threads in execution - - proc bind is set to \"%s\"\n",
	     nthreads, proc_bind_names[binding] );
    }
    int me      = omp_get_thread_num();

    // get on what "place" this thread is running on
    int place   = omp_get_place_num();
    
    // get the number of procs available at this place
    int nprocs  = omp_get_place_num_procs(place);
    
    int proc_ids[nsockets];
    omp_get_place_proc_ids( place, proc_ids );
    
    // get how many places are available in the place list
    int npplaces = omp_get_partition_num_places();

    char **buffer;
    int nbuffers = get_some_work_done_on_sockets( place, &buffer );
    
   #pragma omp for ordered
    for ( int i = 0; i < nsockets; i++)
     #pragma omp ordered
      {
	printf("thread %2d: place %d, nplaces %d, nprocs %d, npplaces %d | procs here are: ",
	       me, place, nsockets, nprocs, npplaces );
	for( int p = 0; p < nprocs; p++ )
	  printf("%d ", proc_ids[p]);	
	printf("\n");	
	for ( int p = 0; p < nbuffers; p++ )   // nbuffers should be equal to nprocs here
	  {
	    printf("%s", buffer[p]);
	    free(buffer[p]);
	  }
      }

    free( buffer );    
    
  }

  return 0;
}



int get_some_work_done_on_sockets( int place, char ***buffer )
{
  #define SIZE 50
  
  int nprocs = omp_get_place_num_procs( place );
  int proc_ids[nprocs];
  omp_get_place_proc_ids( place, proc_ids );

  *buffer = (char**)malloc( nprocs * sizeof(char*) );
  for ( int i = 0; i < nprocs; i++ )
    (*buffer)[i] = (char*)malloc(SIZE );
  
 #pragma omp parallel num_threads(nprocs) proc_bind(close)
  {
    int me = omp_get_thread_num();

    sprintf( (*buffer)[me], "\tL2, place %d, th %2d, nprocs %d\n",
    	     place, me, nprocs );

  }
  
  return nprocs;
}




