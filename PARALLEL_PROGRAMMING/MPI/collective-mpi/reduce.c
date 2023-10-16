#include <stdio.h>
#include <mpi.h>

int main(int argc, char** argv) {

  int num_elements = 2;
  int myid, nproc, root, world_rank;
  double a[num_elements], b[num_elements];
  
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  for (int i = 0; i < num_elements; i++) {
     a[i] = 2.0 * (1 + i * myid);
     fprintf(stdout, "myid=%d: a[%d]=%f\n", myid, i, a[i]);
  }
  root=0;
  //int MPI_Reduce(const void* send_buffer, void* receive_buffer, int count,
  //               MPI_Datatype datatype, MPI_Op operation, int root, MPI_Comm communicator);
  MPI_Reduce(a, b, num_elements, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);
  if (myid == 0) {
    fprintf(stdout,"myid=%d:\n", myid);
    for (int i = 0; i < num_elements; i++)
      fprintf(stdout,"\tb[%d]=%.2f\n", i, b[i]);
  fprintf(stdout,"\n");
  }
  MPI_Finalize();
}
