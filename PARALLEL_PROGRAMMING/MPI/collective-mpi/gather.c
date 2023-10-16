#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

int main(int argc, char** argv) {

  int myid, nproc, root;
  int num_elements = 8;
  int nsnd = 2;
  double *a;
  double *b;
  a = (double*)malloc(sizeof(double) * num_elements);
  b = (double*)malloc(sizeof(double) * nsnd);
  assert(a != NULL);
  assert(b != NULL);
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  int gat_elements = nsnd * nproc;
  root=0;
  if(num_elements < gat_elements && myid == root) {
    printf("This application is meant to be run with no more than %d MPI processes.\n", num_elements/nsnd);
    MPI_Abort(MPI_COMM_WORLD, EXIT_FAILURE);
  }
  for (int i = 0; i < nsnd; i++)
      b[i] = myid;
  // int MPI_Gather(const void* buffer_send, int count_send, MPI_Datatype datatype_send,
  //                void* buffer_recv, int count_recv, MPI_Datatype datatype_recv,
  //                int root, MPI_Comm communicator);
  MPI_Gather(b, nsnd, MPI_DOUBLE, a, nsnd, MPI_DOUBLE, root, MPI_COMM_WORLD);
  if (myid==root) {
    fprintf(stdout, "myid=%d:\n", myid);
    for (int i = 0; i < gat_elements; i++) 
      fprintf(stdout, "\ta[%d]=%.2f\n", i, a[i]);
    fprintf(stdout, "\n");
    for (int i = gat_elements; i < num_elements; i++) 
      fprintf(stdout, "\t\ta[%d]=%.2f\n", i, a[i]);
  }
  free(a);
  free(b);
  MPI_Finalize();
}
