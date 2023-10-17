
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


/* ----------------------------------------------------------------- *
 *                                                                   *
 *                                                                   *
 *                                                                   *
 * ----------------------------------------------------------------- */


#if defined(__STDC__)
#  if (__STDC_VERSION__ >= 201112L)    // c11
#    define _XOPEN_SOURCE 700
#  elif (__STDC_VERSION__ >= 199901L)  // c99
#    define _XOPEN_SOURCE 600
#  else
#    define _XOPEN_SOURCE 500          // c90
#  endif
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#if _XOPEN_SOURCE >= 600
#  include <strings.h>
#endif

char   this_string_goes_in_initialized_global_data[] = "Hello World!\n";
double this_double_goes_in_initialized_global_data   = 3.1415926535897932;

int    function1 ( int, int, char *);
  
int main ( int argc, char **argv )
{

  printf("------------------------------\n"
	 "function: %s\n"
	 "\targc resides at %p\n\n",
	 __FUNCTION__, &argc );
	 
  for ( int ii = 0; ii < argc; ii++ )
    {
      printf("\targ%d's pointer resides at %p\n"
	     "\targ%d resides at %p\n\n",
	     ii,
	     argv + ii,
	     ii, *(argv+ii) );
    }

  int    put_4bytes_in_the_stack;
  double put_8bytes_in_the_stack;
  
  printf("\tput_4bytes_in_the_stack resides at %p\n"
	 "\tput_8bytes_in_the_stack resides at %p\n",
	 &put_4bytes_in_the_stack,
	 &put_8bytes_in_the_stack );
  
  
  function1( 1, 2, this_string_goes_in_initialized_global_data );
  
  return 0;
}


int function1( int iarg1, int iarg2, char *sarg3 )
//
// n.b. just get the point.. the exact placement in registers is
//      compiler dependent
//
{

  int    add_4bytes_to_stack;

  printf("------------------------------\n"
	 "function: %s\n"
	 "\tadd_4bytes_to_stack resides at %p\n"
	 "\tiarg1 and iarg2 stay at %p and %p\n"
	 "\targ3 lives at %p, string lives at %p\n",
	 __FUNCTION__,
	 &add_4bytes_to_stack, &iarg1, &iarg2,
	 sarg3, this_string_goes_in_initialized_global_data);

  return 0;
}

