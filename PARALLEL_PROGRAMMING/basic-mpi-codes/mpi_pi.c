/*
 * Copyright (C) 2015 - 2016 Master in High Performance Computing
 *
 * Adapted from the net by  Giuseppe Brandino. 
 * Last modified by Alberto Sartori. 
 * Added time and promoted to long long all important variables
 * Some more modification by S.Cozzini (nov 2021)
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <mpi.h>
#define USE MPI
#define SEED 35791246

int main (int argc , char *argv[])
{
  // coordinates
  double x, y;

  // number of points inside the circle
  long long int M, local_M; 
  double pi;
   
  // times 
  double start_time, comp_time, end_time, wall_time, avg_walltime, max_walltime;   
  int myid, numprocs, proc;
  MPI_Status status;
  MPI_Request request;
  // master process
  int master = 0;
  int tag = 123;

  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
  MPI_Comm_rank(MPI_COMM_WORLD, &myid);
  fprintf (stdout, "I am  %d\n", myid);
  if (argc <=1 ) {
    fprintf(stderr, "Usage : mpi -np n %s number_of_iterations \n", argv[0]);
    MPI_Finalize();
    exit(-1);
  }

  long long int N = atoll(argv[1])/numprocs;
  // take time of processors after initial I/O operation
  start_time = MPI_Wtime();

  // initialize random numbers 
  srand48(SEED * (myid + 1)); // seed the number generator
  local_M = 0;
  long long int i;
  for (i = 0; i < N ; i++) {
    // take a point P(x,y) inside the unit square
    x = drand48(); 
    y = drand48();
    // check if the point P(x,y) is inside the circle
    if ( (x*x + y*y) < 1)
      local_M++;
  }
  // take time of processors after initial I/O operation
  MPI_Barrier(MPI_COMM_WORLD);
  comp_time=MPI_Wtime();

  if (myid == 0) { //if I am the master process gather results from others
    M = local_M;
    for (proc = 1; proc < numprocs; proc++) {
      MPI_Recv(&local_M, 1, MPI_LONG_LONG, proc, tag, MPI_COMM_WORLD, &status);
      M += local_M;
    }
    pi = 4.0 * M / (N * numprocs);
    end_time = MPI_Wtime();
  }
  else {   // for all the slave processes send results to the master /
    MPI_Ssend(&local_M, 1,MPI_LONG_LONG, master, tag, MPI_COMM_WORLD);
    end_time=MPI_Wtime();
  }
 
  wall_time = end_time - start_time;
  MPI_Reduce(&wall_time, &avg_walltime, 1, MPI_DOUBLE, MPI_SUM, master, MPI_COMM_WORLD);
  avg_walltime = avg_walltime / numprocs;
  MPI_Reduce(&wall_time, &max_walltime, 1, MPI_DOUBLE, MPI_MAX, master, MPI_COMM_WORLD);

  fprintf(stdout, "\n# walltime on processor %i : %10.8f\n", myid, wall_time);
  fprintf(stdout, "\n# walltime after computation on processor %i : %10.8f\n", myid, comp_time - start_time);
  fprintf(stdout, "\n# walltime for communication on processor %i : %10.8f\n", myid, end_time - comp_time);
  fflush(stdout);
  if (myid ==0) { 
    printf ( "\n# of trials = %llu , estimate of pi is %1.9f\n", N * numprocs, pi);
    fprintf(stdout, "\n[*] Average Walltime: %10.8f\n", avg_walltime);
    fprintf(stdout, "\n(*) Max Walltime: %10.8f\n", max_walltime);
    fflush(stdout);
  } 
  MPI_Finalize() ; // let MPI finish up /

}
