#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <string.h>
int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  int rank, size;
  int buffer;
  MPI_Status status;
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  if(size != 2) {
    printf("This application is meant to be run with 2 processes.\n");
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
  if (rank == 0){
    // int MPI_Recv(void* buffer, int count, MPI_Datatype datatype,
    //              int sender, int tag, MPI_Comm communicator, MPI_Status* status);
    MPI_Recv(&buffer, 1, MPI_INT, 1, 9, MPI_COMM_WORLD, &status);
    fprintf(stdout, "Rank %d: buffer = %d \n", rank, buffer);
    if (buffer != 33) fprintf(stderr, "Fail\n");
  }
  if (rank == 1) {
    buffer = 33;
    // int MPI_Send(const void* buffer, int count, MPI_Datatype datatype,
    //              int recipient, int tag, MPI_Comm communicator);
    MPI_Send(&buffer, 1, MPI_INT, 0, 9, MPI_COMM_WORLD);
  }
  MPI_Finalize();
  return EXIT_SUCCESS;
}

