#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#include <assert.h>

#define D 2

struct dims {
    int d[D];
};

void compute_local_dims(struct dims * ldims, const struct dims gdims) {
  int myid, nproc;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  struct dims r;
  for (int i = 0; i < D; i++) 
    r.d[i] = 0;
  for (int p = 0; p < nproc ; p++) {
    for (int i = 0; i < D; i++) {
      ldims[p].d[i] = gdims.d[i] / nproc;
      r.d[i] = p < gdims.d[i] % nproc? 1 : 0;
      ldims[p].d[i] += r.d[i];
    }
  }
}

void init(int ** a, const struct dims gdims) {
  int myid, nproc;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  struct dims ldims[nproc];
  compute_local_dims(ldims, gdims);
  int rank   = 0;
  int cumsum = 0;
  while( rank < myid) {
    cumsum += ldims[rank].d[0] * gdims.d[1];
    rank++;
  }
  for (int i = 0; i < gdims.d[1] * ldims[myid].d[0]; i++)
    (*a)[i] = cumsum + i;
}

void print(const int * a, const struct dims gdims) {
  int myid, nproc;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  struct dims ldims[nproc]; 
  compute_local_dims(ldims, gdims);

  for (int p = 0; p < nproc; p++) {
    if (p == myid) {
      for (int i = 0 ; i < ldims[p].d[0]; i++) { 
        for (int j = 0 ; j < gdims.d[1]; j++) 
          fprintf(stdout, "%d\t", a[ i * gdims.d[1] + j]);
        fprintf(stdout, "\n");
      }
      fflush(stdout);
      sleep(0.1);
    }
    MPI_Barrier(MPI_COMM_WORLD);
  }
  if (myid==0) fprintf(stdout, "\n");
  MPI_Barrier(MPI_COMM_WORLD);
}

void transpose(const int *a, const struct dims *gdims_a, int *b, struct dims *gdims_b) 
{
  int myid, nproc;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  struct dims ldims[nproc];
  compute_local_dims(ldims, *gdims_a);

  int counts_send[nproc];
  int displacements_send[nproc];
  int counts_recv[nproc];
  int displacements_recv[nproc];
  int shifts_n1[nproc];
  int shifts_n2[nproc];
  for (int p = 0; p < nproc ; p++) {
    counts_send[p] = ldims[myid].d[0] * ldims[p].d[1];
    counts_recv[p] = ldims[p].d[0] * ldims[myid].d[1];
    displacements_send[p] = p == 0 ? 0 : displacements_send[p-1] + counts_send[p-1];
    displacements_recv[p] = p == 0 ? 0 : displacements_recv[p-1] + counts_recv[p-1];
    shifts_n1[p] = p == 0 ? 0 : counts_send[p-1] + shifts_n1[p-1];
    shifts_n2[p] = p == 0 ? 0 : ldims[p-1].d[1] + shifts_n2[p-1];
  }
  
  int buffer_send_size = gdims_a->d[1] * ldims[myid].d[0];
  int buffer_recv_size = gdims_a->d[0] * ldims[myid].d[1];
  int delta1 = 0;
  int delta2 = 0;
  int index_buf, index_mat;
  int * buffer_send=(int *) malloc(sizeof(int) * buffer_send_size);
  int * buffer_recv=(int *) malloc(sizeof(int) * buffer_recv_size);
  
  for (int p = 0;  p < nproc; p++) {
    for (int i = 0;  i < ldims[myid].d[0]; i++) {
      for (int j = 0;  j < ldims[p].d[1]; j++) {
        index_buf = i * ldims[p].d[1] + j + shifts_n1[p];
        index_mat = i * gdims_a->d[1] + j + shifts_n2[p];
	buffer_send[index_buf] = a[index_mat];
	//buffer_send[i * ldims[p].d[1] + j + delta1] = a[i * gdims_a->d[1] + j + delta2];
      }
    }
    //delta1 += ldims[p].d[1] * ldims[myid].d[0];
    //delta2 += ldims[p].d[1]; 
  }
 
  MPI_Alltoallv(buffer_send, counts_send, displacements_send, MPI_INT, 
		buffer_recv, counts_recv, displacements_recv, MPI_INT, 
		MPI_COMM_WORLD);
  
  for (int j = 0;  j < gdims_a->d[0]; j++) {
    for (int i = 0;  i < ldims[myid].d[1]; i++) {
      index_buf = j * ldims[myid].d[1] + i;
      index_mat = i * gdims_a->d[0] + j;
      b[index_mat] = buffer_recv[index_buf];
    }
  }
  gdims_b->d[0] = gdims_a->d[1];
  gdims_b->d[1] = gdims_a->d[0];
  free(buffer_send);
  free(buffer_recv);
}


int main(int argc, char** argv) {

  int myid, nproc;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  struct dims gdims_a;
  struct dims gdims_b;
  struct dims gdims_c;
  struct dims ldims_a[nproc];
  gdims_a.d[0] = 9;
  gdims_a.d[1] = 10;
  compute_local_dims(ldims_a, gdims_a);
  int * a = (int *) malloc(sizeof(int) * gdims_a.d[1] * ldims_a[myid].d[0]);
  int * b = (int *) malloc(sizeof(int) * gdims_a.d[0] * ldims_a[myid].d[1]);

  MPI_Barrier(MPI_COMM_WORLD);
  if (myid==0) {
    fprintf(stdout, "\nA is a %dx%d matrix with contiguous data in the 2nd and then in the 1st dimension \n\n", gdims_a.d[0], gdims_a.d[1]);
    fflush(stdout);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  init(&a, gdims_a);
  print(a, gdims_a);


  MPI_Barrier(MPI_COMM_WORLD);
  transpose(a, &gdims_a, b, &gdims_b);
  if (myid==0) {
    fprintf(stdout, "\nTranspose\n\n");
    fprintf(stdout, "\nB is a %dx%d matrix with contiguous data in the 1st and then in the 2nd  dimension \n\n", gdims_b.d[0], gdims_b.d[1]);
    fflush(stdout);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  print(b, gdims_b);


	/*
  int myid, nproc;
  MPI_Init(NULL, NULL);
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  struct dims gdims;
  struct dims ldims[nproc];
  gdims.d[0] = 11;
  gdims.d[1] = 7;
  compute_local_dims(ldims, gdims); 
  int * a = (int *) malloc(sizeof(int) * gdims.d[1] * ldims[myid].d[0]);
  int * b;
  init(&a, gdims);
  print(a, gdims);
  
  b = transpose(a, &gdims);
  print(b, gdims);
  
  free(a);
  a = transpose(b, &gdims);
  print(a, gdims);
  */
  free(a);
  free(b);
  MPI_Finalize();
}
