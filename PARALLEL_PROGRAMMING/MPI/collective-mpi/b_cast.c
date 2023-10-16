#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <assert.h>

int main(int argc, char** argv) {

  int num_elements = 2;
  int myid, nproc, root;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  double * a = (double*)malloc(sizeof(double) * num_elements);
  assert(a != NULL);
  for (int i=0; i < num_elements; i++) {
    a[i]=0.;
  }
  root = 0;
  if (myid == root) {  
    for (int i = 0 ; i < num_elements; i++)
      a[i] = 2. * (i + 1.);
  }
  for (int i = 0; i < nproc; i++) {
    if (i == myid) {
      fprintf(stdout, "%d\tbefore:", myid );
      for (int n = 0 ; n < num_elements; n++) 
        fprintf(stdout, "\ta[%d]=%.2f ", n, a[n]);
      fprintf(stdout, "\n");
      fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  // int MPI_Bcast(void* buffer, int count, MPI_Datatype datatype, int emitter_rank, MPI_Comm communicator);
  MPI_Bcast(a, num_elements, MPI_DOUBLE, 0, MPI_COMM_WORLD);
  for (int i = 0; i < nproc; i++) {
    if (i == myid) {
      fprintf(stdout, "%d\tafter:", myid );
      for (int n = 0 ; n < num_elements; n++) 
        fprintf(stdout, "\ta[%d]=%.2f ", n, a[n]);
      fprintf(stdout, "\n");
      fflush(stdout);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  free(a);
  MPI_Finalize();
}
