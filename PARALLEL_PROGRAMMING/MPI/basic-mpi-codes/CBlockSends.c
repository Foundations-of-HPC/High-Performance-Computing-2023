// Taken from https://cvw.cac.cornell.edu/MPIP2P/CBlockSends //
#include <stdlib.h>
#include <stdio.h>
#include "mpi.h"
#define NMSG 4
#define SYNC 0
#define READY 1
#define BUF 2
#define STANDARD 3
#define BUF_ALLOC 4
#define GO 10
void print_time(int event, double tbegin, double tend);

int main(int argc, char *argv[] )
{
  float *message [NMSG];          /* pointers to messages                 */
  float *buffer;                  /* buffer required by buffered send     */
  int rank,                       /* rank of task in communicator         */
      count,                      /* number of elements in message        */
      dest,                       /* rank in communicator of destination  */
      source,                     /* and source tasks                     */
      tag[NMSG], etag,            /* message tags                         */
      mlen,                       /* message length specified by user     */
      bsize,                      /* buffer size for buffered send, bytes */
      i, j;
  double tbegin [NMSG+1],         /* used to measure elapsed time         */
         tend   [NMSG+1];
  
  MPI_Comm comm;                  /* communicator                         */
  MPI_Datatype datatype;          /* type of data in message              */
  MPI_Request request[NMSG];      /* handle for pending communication     */
  MPI_Status status[NMSG];        /* status of communication              */

  if (argc != 2) {
    printf ( " Usage: blocksends <message_length_in_number_of_floats>\n" );
    return -1;
  }

  /* -------------------------------------------------------------------
   * do initial housekeeping: allocate memory for messages,
   * initialize program with MPI, define message tags 
   * ------------------------------------------------------------------ 
   */

  mlen = atoi (argv[1]);
  for (i = 0; i < NMSG; i++)
    message[i] = (float *)malloc ( mlen * sizeof(float) );
  
  MPI_Init( &argc, &argv );
  MPI_Comm_rank( MPI_COMM_WORLD, &rank );
  printf ( " Message size = %5d floats\n", mlen );
  printf ( " Task %d initialized\n", rank );

  tag[SYNC] = SYNC;
  tag[READY] = READY;
  tag[BUF] = BUF;
  tag[STANDARD] = STANDARD;

  /*  -------------------------------------------------------------------
   *  task 0 will receive a ready message, and then send 4 messages using
   *  the different communication modes 
   *  ------------------------------------------------------------------ 
   */  
   if (rank == 0) {
    /* initialize all message contents */
    for (i = 0; i < NMSG; i++)
      for (j = 0; j < mlen; j++)
        message[i][j] = (float) i;

    /* receive empty message, indicating all receives have been posted */
    count = 0;
    datatype = MPI_INT;
    source = 1; 
    etag = GO;
    comm = MPI_COMM_WORLD;
    MPI_Recv((int*) 0, count, datatype, source, etag, comm, &status[1]);
    printf(" Ready to send messages\n");

    /* these message parameters apply to all succeeding sends by task 0 */
    count = mlen;         
    datatype = MPI_FLOAT;
    dest = 1;
    comm = MPI_COMM_WORLD;

    /* time a synchronous send */
   
    tbegin[SYNC] = MPI_Wtime();
    MPI_Ssend(message[SYNC], count, datatype, dest, tag[SYNC], comm);
    tend[SYNC] = MPI_Wtime();   
    print_time(SYNC, tbegin[SYNC], tend[SYNC]);

    /* time a ready send */
   
    tbegin[READY] = MPI_Wtime();
    MPI_Rsend(message[READY], count, datatype, dest, tag[READY], comm);
    tend[READY] = MPI_Wtime();   
    print_time(READY, tbegin[READY], tend[READY]);

    /* time overhead for buffer allocation for buffered send */
   
    tbegin[BUF_ALLOC] = MPI_Wtime();
    bsize  =  count *  sizeof (float) + 100;
    buffer = (float *) malloc (bsize);
    MPI_Buffer_attach(buffer, bsize);
    tend[BUF_ALLOC] = MPI_Wtime();   
    print_time(BUF_ALLOC, tbegin[BUF_ALLOC], tend[BUF_ALLOC]);

    /* time a buffered send */   
   
    tbegin[BUF] = MPI_Wtime();
    MPI_Bsend(message[BUF], count, datatype, dest, tag[BUF], comm);
    tend[BUF] = MPI_Wtime();   
    print_time(BUF, tbegin[BUF], tend[BUF]);
    MPI_Buffer_detach(buffer, &bsize);

    /* time a standard send */
   
    tbegin[STANDARD] = MPI_Wtime();
    MPI_Send(message[STANDARD], count, datatype, dest, tag[STANDARD], comm);
    tend[STANDARD] = MPI_Wtime();
    print_time(STANDARD, tbegin[STANDARD], tend[STANDARD]);
  }
  
  /*  -------------------------------------------------------------------
   *  task 1 will post 4 receives, and then send a "ready" message
   *  ------------------------------------------------------------------ 
   */  

  else if ( rank == 1 )  {
    count = mlen;
    datatype = MPI_FLOAT;
    source = 0;
    comm = MPI_COMM_WORLD;

    /* post non-blocking receives */
    for (i = 0; i < NMSG; i++)
      MPI_Irecv(message[i], count, datatype, source, tag[i], comm, &request[i]);

    /* send ready message indicating all receives have been posted */
    count = 0;
    datatype = MPI_INT;
    dest = 0;
    etag = GO;   
   
    MPI_Send( (int *) 0, count, datatype, dest, etag, comm);

    /* wait for all receives to complete */
    MPI_Waitall(NMSG, request, status);
  }
  MPI_Finalize();
  return 0;
}

/* -------------------------------------------------------------------
 * calculate elapsed time and print results
 * ------------------------------------------------------------------- 
 */
void print_time(int event, double tbegin, double tend)
{
  int dt;
  dt = (int) 1000000. * (tend - tbegin);

  switch (event)  {
  case SYNC: 
    printf(" Elapsed time for synchronous send = %8d uSec\n", dt );
    break;
  case READY: 
    printf(" Elapsed time for ready send = %8d uSec\n", dt );
    break;
  case BUF: 
    printf(" Elapsed time for buffered send = %8d uSec\n", dt );
    break;
  case STANDARD: 
    printf(" Elapsed time for standard send = %8d uSec\n", dt );
    break;
  case BUF_ALLOC: 
    printf(" Elapsed time for buffer allocation = %8d uSec\n", dt );
    break;
  default:
    break;
  }
}
