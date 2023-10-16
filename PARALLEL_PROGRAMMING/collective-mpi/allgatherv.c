#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#include <assert.h>
#define SEED 35791246

int main(int argc, char** argv) {

  int myid, nproc;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  srand(SEED*(myid+1)); // seed the number generator
  int numel = 1 + (rand() % 9);
  int totel;
  
  int counts_recv[nproc];
  int displacements[nproc];
  MPI_Allgather(&numel, 1, MPI_INT, counts_recv, 1, MPI_INT, MPI_COMM_WORLD);
  displacements[0] = 0 ;
  for (int i = 1; i < nproc ; i++){
    displacements[i] = displacements[i-1] + counts_recv[i-1];
  }
  
  double * a = (double*)malloc(sizeof(double) * numel);
  assert(a != NULL);

  for (int i=0; i < numel; i++) {
    a[i]=myid;
  }
  
  for (int i = 0; i < nproc; i++) {
    if (i == myid) {
      fprintf(stdout, "BEFORE\tmyid = %d\n", myid );
      for (int n = 0 ; n < numel; n++) 
        fprintf(stdout, "\ta[%d]=%.1f\n", n, a[n]);
      fprintf(stdout, "\n");
      fflush(stdout);
    }
    sleep(0.01);
    MPI_Barrier(MPI_COMM_WORLD);
  }
  
  MPI_Allreduce(&numel, &totel, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
  double * b = (double*)malloc(sizeof(double) * totel);
  assert(b != NULL);
  
  MPI_Allgatherv(a, numel, MPI_DOUBLE, b, counts_recv, displacements, MPI_DOUBLE, MPI_COMM_WORLD);
  for (int i = 0; i < nproc; i++) {
    if (i == myid) {
      fprintf(stdout, "AFTER\tmyid = %d\n", myid );
      for (int n = 0 ; n < totel; n++) 
        fprintf(stdout, "\tb[%d]=%.1f\n", n, b[n]);
      fprintf(stdout, "\n");
      fflush(stdout);
    }
    sleep(0.01);
    MPI_Barrier(MPI_COMM_WORLD);
  }
  free(a);
  free(b);
  MPI_Finalize();
}
