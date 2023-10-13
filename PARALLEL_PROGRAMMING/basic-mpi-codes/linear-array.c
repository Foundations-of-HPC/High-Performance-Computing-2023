// A simple 1-D example:
// each element receive the rank of the previous one, add its rank and send forward. 
// taken somewhere on net and adapted.
// Final SUM is the sum of n-1 integers.

#include <stdio.h>
#include "mpi.h"

int main(int argc,char *argv[])
{
  int MyRank, Numprocs;
  int value, sum = 0;
  int Source, Source_tag;
  int Destination, Destination_tag;
  int Root = 0;
  MPI_Status status;

  MPI_Init(&argc,&argv);
  MPI_Comm_size(MPI_COMM_WORLD, &Numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &MyRank);
   
  if (MyRank == Root) {
    Destination     = MyRank + 1;
    Destination_tag = 0;
    MPI_Send(&MyRank, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD);
  }
  else {     
    if (MyRank<Numprocs - 1) {
      Source     = MyRank - 1;
      Source_tag = 0;
      
      MPI_Recv(&value, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &status);
      
      sum = MyRank + value;
      Destination     = MyRank + 1;
      Destination_tag = 0;
      MPI_Send(&sum, 1, MPI_INT, Destination, Destination_tag, MPI_COMM_WORLD);
    }
    else {
      Source     = MyRank - 1;
      Source_tag = 0;
      
      MPI_Recv(&value, 1, MPI_INT, Source, Source_tag, MPI_COMM_WORLD, &status);
      
      sum = MyRank + value;
      printf("MyRank %d Final SUM %d\n", MyRank, sum);
    }
  }
  
  MPI_Finalize();
  
}

