#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#include <assert.h>

#define D 3

struct dims {
  int d[D];
};

int index_f (int i1, int i2, int i3, int n1, int n2, int n3) {
  return n3 * n2 * i1 + n3 * i2 + i3;
}

void compute_local_dims(struct dims * ldims, const struct dims gdims) {
  int myid, nproc, p, i;
  struct dims r;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  for (p = 0; p < nproc ; p++) {
    for (i = 0; i < D; i++) {
      ldims[p].d[i] = gdims.d[i] / nproc;
      r.d[i] = p < (gdims.d[i] % nproc)? 1 : 0;
      ldims[p].d[i] += r.d[i];
    }
  }
}

void init(int ** a, const struct dims gdims) {
  int myid, nproc, i;
  int rank   = 0;
  int cumsum = 0;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  struct dims ldims[nproc];
  compute_local_dims(ldims, gdims);
  while(rank < myid) {
    cumsum += ldims[rank].d[0] * gdims.d[1] * gdims.d[2];
    rank++;
  }
  for (i = 0; i < ldims[myid].d[0] * gdims.d[1] * gdims.d[2]; i++)
    (*a)[i] = cumsum + i;
}

void print(const int * a, const struct dims gdims) {
  int myid, nproc, p, i, j, k, index;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  struct dims ldims[nproc]; 
  compute_local_dims(ldims, gdims);

  for (p = 0; p < nproc; p++) {
    if (p == myid) {
      for (i = 0 ; i < ldims[p].d[0]; i++) {
        for (j = 0 ; j < gdims.d[1]; j++) {
          for (k = 0 ; k < gdims.d[2]; k++) {
            index = index_f(i, j, k, ldims[myid].d[0], gdims.d[1], gdims.d[2]);
            fprintf(stdout, "%d\t", a[index]);
	  }
	  fprintf(stdout, "\n");
        }
	fprintf(stdout, "\n");
      }
      fflush(stdout);
    }
    sleep(0.01);
    MPI_Barrier(MPI_COMM_WORLD);
  }

  if (myid==0) fprintf(stdout, "\n");
  sleep(0.01);
  MPI_Barrier(MPI_COMM_WORLD);
}

void transpose_2_1(const int *a, const struct dims *gdims_a, int *b, struct dims *gdims_b) 
{
  int myid, nproc, index_buf, index_mat, p, i, j, k;
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
  for (p = 0; p < nproc ; p++) {
    counts_send[p] = ldims[myid].d[0] * ldims[p].d[1] * gdims_a->d[2];
    counts_recv[p] = ldims[p].d[0] * ldims[myid].d[1] * gdims_a->d[2];
    displacements_send[p] = p == 0 ? 0 : displacements_send[p-1] + counts_send[p-1];
    displacements_recv[p] = p == 0 ? 0 : displacements_recv[p-1] + counts_recv[p-1];
    shifts_n1[p] = p == 0 ? 0 : counts_send[p-1] + shifts_n1[p-1];
    shifts_n2[p] = p == 0 ? 0 : gdims_a->d[2] * ldims[p-1].d[1] + shifts_n2[p-1];
  }

  int buffer_send_size = gdims_a->d[2] * gdims_a->d[1] * ldims[myid].d[0];
  int buffer_recv_size = gdims_a->d[2] * gdims_a->d[0] * ldims[myid].d[1];
  int * buffer_send = (int *) malloc(sizeof(int) * buffer_send_size);
  int * buffer_recv = (int *) malloc(sizeof(int) * buffer_recv_size);

  for (p = 0;  p < nproc; p++) {
    for (i = 0;  i < ldims[myid].d[0]; i++) {
      for (j = 0;  j < ldims[p].d[1]; j++) {
        for (k = 0;  k < gdims_a->d[2]; k++) {
	index_buf = index_f(i,j,k+shifts_n1[p],gdims_a->d[0],ldims[p].d[1],gdims_a->d[2]);
	index_mat = index_f(i,j,k+shifts_n2[p],ldims[myid].d[0],gdims_a->d[1],gdims_a->d[2]);
	buffer_send[index_buf] = a[index_mat];
        }
      }
    }
  }
 
  MPI_Alltoallv(buffer_send, counts_send, displacements_send, MPI_INT, 
		buffer_recv, counts_recv, displacements_recv, MPI_INT, 
		MPI_COMM_WORLD);

  for (j = 0;  j < gdims_a->d[0]; j++) {
    for (i = 0;  i < ldims[myid].d[1]; i++) {
      for (k = 0;  k < gdims_a->d[2]; k++) {
        index_mat = index_f(i, j, k, ldims[myid].d[1], gdims_a->d[0], gdims_a->d[2]);
        index_buf = index_f(j, i, k, gdims_a->d[0], ldims[myid].d[1], gdims_a->d[2]);
        b[index_mat] = buffer_recv[index_buf];	
      }
    }
  }

  gdims_b->d[0] = gdims_a->d[1];
  gdims_b->d[1] = gdims_a->d[0];
  gdims_b->d[2] = gdims_a->d[2];
  free(buffer_send);
  free(buffer_recv);
}

void transpose_3_2(const int *a, const struct dims *gdims_a, int *b, struct dims *gdims_b) 
{
  int myid, nproc, index_a, index_b, i, j, k;
  MPI_Comm_size(MPI_COMM_WORLD, &nproc);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  struct dims ldims_a[nproc];
  compute_local_dims(ldims_a, *gdims_a);
  for (i = 0;  i < ldims_a[myid].d[0]; i++) {
    for (j = 0;  j < gdims_a->d[1]; j++) {
      for (k = 0;  k < gdims_a->d[2]; k++) {
        index_b = index_f(i, k, j, ldims_a[myid].d[0], gdims_a->d[2], gdims_a->d[1]);
        index_a = index_f(i, j, k, ldims_a[myid].d[0], gdims_a->d[1], gdims_a->d[2]);
	b[index_b] = a[index_a];
      }
    }
  }
  
  gdims_b->d[2] = gdims_a->d[1];
  gdims_b->d[1] = gdims_a->d[2];
  gdims_b->d[0] = gdims_a->d[0];  
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
  gdims_a.d[2] = 11;
  compute_local_dims(ldims_a, gdims_a); 
  int * a = (int *) malloc(sizeof(int) * gdims_a.d[2] * gdims_a.d[1] * ldims_a[myid].d[0]);
  int * b = (int *) malloc(sizeof(int) * gdims_a.d[2] * gdims_a.d[1] * ldims_a[myid].d[0]);
  int * c = (int *) malloc(sizeof(int) * gdims_a.d[2] * gdims_a.d[0] * ldims_a[myid].d[1]);

  MPI_Barrier(MPI_COMM_WORLD);
  if (myid==0) {
    fprintf(stdout, "\nA is a %dx%dx%d matrix with contiguos data in the 3rd, then 2nd, and finally in the 1st dimension \n\n", gdims_a.d[0], gdims_a.d[1], gdims_a.d[2]);
    fflush(stdout);
  }
  MPI_Barrier(MPI_COMM_WORLD);

  init(&a, gdims_a);
  print(a, gdims_a);
  
  MPI_Barrier(MPI_COMM_WORLD);
  transpose_3_2(a, &gdims_a, b, &gdims_b);
  if (myid==0) { 
    fprintf(stdout, "\nTranspose 3 -> 2\n\n");
    fprintf(stdout, "\nB is a %dx%dx%d matrix with contiguos data in the 2nd, then 3rd, and finally in the 1st dimension \n\n", gdims_b.d[0], gdims_b.d[1], gdims_b.d[2]);
    fflush(stdout);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  print(b, gdims_b);
  
  MPI_Barrier(MPI_COMM_WORLD);
  transpose_2_1(a, &gdims_a, c, &gdims_c);
  if (myid==0) { 
    fprintf(stdout, "\nTranspose 2 -> 1\n\n");
    fprintf(stdout, "\nC is a %dx%dx%d matrix with contiguos data in the 3rd, then 1st, and finally in the 2nd dimension \n\n", gdims_c.d[0], gdims_c.d[1], gdims_c.d[2]);
    fflush(stdout);
  }
  MPI_Barrier(MPI_COMM_WORLD);
  print(c, gdims_c);
  
  free(a);
  free(b);
  free(c);
  MPI_Finalize();
}
