#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  int buffer;
  char message[2][16];
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 2) {
    printf("This application is meant to be run with 2 processes.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
  if (rank == 0){
    strcpy(message[0], "skew");
    strcpy(message[1], "squeue");
    // int MPI_Sendrecv(const void* buffer_send, int count_send, 
    //                  MPI_Datatype datatype_send, int recipient, int tag_send,
    //                  void* buffer_recv, int count_recv,
    //                  MPI_Datatype datatype_recv, int sender, int tag_recv,
    //                  MPI_Comm communicator, MPI_Status* status);
    MPI_Sendrecv(message, 32, MPI_CHAR, 1, 10, 
		 &buffer,  1, MPI_INT,  1,  9, 
		 MPI_COMM_WORLD, &status);
    fprintf(stdout, "Rank %d: buffer = %d \n", rank, buffer);
    if (buffer != 33) fprintf(stderr, "Fail\n");
  }
  if (rank == 1) {
    buffer = 33;	  
    MPI_Sendrecv(&buffer,  1,  MPI_INT, 0,  9,
                 message, 32, MPI_CHAR, 0, 10, 
                 MPI_COMM_WORLD, &status);
    
    fprintf(stdout, "Rank %d: message[0] = %s, message[1] = %s \n", 
		    rank, message[0], message[1]);
  }
  MPI_Finalize();
  return EXIT_SUCCESS;
}

