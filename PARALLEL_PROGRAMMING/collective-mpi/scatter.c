#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>




int main(int argc, char** argv) {

  int myid, nproc, root;
  int num_elements = 8;
  int nsnd = 2;
  double a[num_elements];
  double *b;
  b = (double*)malloc(sizeof(double) * nsnd);
  assert(b != NULL);
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  root=0;
  if(nproc * nsnd != num_elements && myid == root) {
        printf("This application is meant to be run with %d MPI processes.\n", num_elements/nsnd);
        MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
  if (myid == root) {
    for (int i = 0; i < num_elements; i++)
      a[i] = i+1;
  }
  // int MPI_Scatter(const void* buffer_send, int count_send, MPI_Datatype datatype_send,
  //                 void* buffer_recv, int count_recv, MPI_Datatype datatype_recv,
  //                 int root, MPI_Comm communicator);

  MPI_Scatter(a, nsnd, MPI_DOUBLE, b, nsnd, MPI_DOUBLE, root, MPI_COMM_WORLD);
  fprintf(stdout, "myid=%d:\tb[0]=%.2f,\tb[1]=%.2f\n",myid, b[0], b[1] );
  free(b);
  MPI_Finalize();
}
