// taken from https://cvw.cac.cornell.edu/MPIP2P/brecv
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "mpi.h"
#define TAG 100

void print_time(double tbegin, double tend);
int new_sleep(int);
int SLEEP(clock_t);

/* -------------------------------------------------------------------
 * helper to calculate elapsed time and print results
 * ------------------------------------------------------------------- 
 */
void print_time(double tbegin, double tend)
{
  int dt;
  dt = (int)((tend - tbegin) * 1000000.0);
  printf(" Elapsed time for send = %8d uSec\n", dt);
}

/* -----------------------------------------------------------
 * helpers to sleep program
 * ----------------------------------------------------------- 
 */
int SLEEP(clock_t wait)
{
    clock_t goal;
    wait *= 1000;
    goal = wait + clock();
    while (goal >  clock() )
      ;
    return (0);
}

int new_sleep(int amount)
{
  SLEEP(amount);
  return (0);
}

/* -----------------------------------------------------------
 * Main Program
 * ----------------------------------------------------------- 
 */
int main(int argc, char **argv)
{
  float *message;            /* message buffer               */
  int rank,                  /* rank of task in communicator */
      size, i;
  int mlen;                  /* dimension of the message     */
  MPI_Status status;         /* status of communication      */
  double tbegin, tend;       /* used to measure elapsed time */

  if (argc != 2) {
    printf(" Usage: blocksends <message_length_in_number_of_floats>\n");
    return -1;
  }

/* -------------------------------------------------------------------
 * do initial housekeeping: allocate memory for messages,
 * initialize program with MPI, define message tags
 * ------------------------------------------------------------------ 
 */

  mlen = atoi(argv[1]);
  message = (float *)malloc(mlen * sizeof(float));


  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  if(size != 2) {
    printf("This application is meant to be run with 2 processes.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
  printf(" Process %d initialized\n", rank);
  printf(" Message size = %6d floats\n", mlen);
  printf(" Total size = %6lu bytes\n", (mlen* sizeof(float)));

/* -----------------------------------------------------------------
 * task 0 will report the elapsed time for a blocking send
 * ----------------------------------------------------------------- 
 */
  if (rank == 0)  {
    for (i = 0; i < mlen; i++) message[i] = 100;
    printf(" Task %d sending message\n", rank);
    MPI_Barrier(MPI_COMM_WORLD);
    tbegin = MPI_Wtime();
    MPI_Send(message, mlen, MPI_FLOAT, 1, TAG, MPI_COMM_WORLD);
    tend = MPI_Wtime();
    print_time(tbegin, tend);
  }

/* -----------------------------------------------------------------
 * task 1 sleeps for 1 second, and then calls a blocking receive.
 * the sleep is intended to simulate time spent in useful computation
 * ----------------------------------------------------------------- 
 */
  else if (rank == 1)  {
    for (i = 0; i < mlen; i++) message[i] = -100;
    MPI_Barrier(MPI_COMM_WORLD);
    new_sleep(1);
    MPI_Recv(message, mlen, MPI_FLOAT, 0, TAG, MPI_COMM_WORLD, &status );
    printf(" Task %d received message\n", rank);
  }
  MPI_Finalize();
  return 0;
}


