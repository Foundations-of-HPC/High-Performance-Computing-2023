# basic and simple examples for MPI collective operations

Let's allocate 8 tasks on one EPYC node in interactive way by means of

`$> salloc -n8 -N1 -p EPYC --time=2:00:00 --mem=10GB`

Then load openMPI using

`$> module load openMPI/4.1.5/gnu`

Check that the module is properly load usign

`$> module list`

Compile and run 

```
$> mpicc b_cast.c -o b_cast.x
$> mpirun -np 4 ./b_cast.x 
$> mpirun -np 8 ./b_cast.x 
```

What is going on? How does the MPI_Bcast function work?
 
Modify the line 18

`root = 0;`

with 

`root = 1;`

Compile and run

```
$> mpicc b_cast.c -o b_cast.x
$> mpirun -np 4 ./b_cast.x
```

What happens? What has changed?

Another perspective on the `MPI_Bcast` can be found in `mpi_bcastcompare.c`, where we compare `MPI_Bcast` with an implementation from scratch based on `MPI_Send` and `MPI_Recv`.

```
$> mpicc mpi_bcastcompare.c -o mpi_bcastcompare.x 
$> mpirun -np 8 ./mpi_bcastcompare.x
```

Which is faster `my_bcast` or `MPI_Bcast`?

Compile and run

```
$> mpicc reduce.c -o reduce.x
$> mpirun -np 8 ./reduce.x
```

What is the output? Why?

Modify line 20

`MPI_Reduce(a, b, num_elements, MPI_DOUBLE, MPI_SUM, root, MPI_COMM_WORLD);`

with 

`MPI_Reduce(a, b, num_elements, MPI_DOUBLE, MPI_MIN, root, MPI_COMM_WORLD);`

Compile and run

```
$> mpicc reduce.c -o reduce.x
$> mpirun -np 8 ./reduce.x
```

What happens? Why?

Compile and run

```
$> mpicc scatter.c -o scatter.x
$> mpirun -np 8 ./scatter.x
```

What happens?

Try:

`$> mpirun -np 4 ./scatter.x`

What does the function `MPI_Scatter` do? Can you understand the output?

Compile and run 

```
$> mpicc gather.c -o gather.x
$> mpirun -np 4 ./gather.x  
```

What happens?

Try also

```
$> mpirun -np 3 ./gather.x 
$> mpirun -np 2 ./gather.x 
```

What do you notice? Why?

In `gather.c` we gather on root arrays of a given number of elements.

In `allgatherv.c`, instead of gathering the arrays on root, we allgather on all ranks. 
That is every rank will gather the arrays. 

Moreover, we gather arrays with a different numbers of elements (chosen randomly).

Compile and run

```
$> mpicc allgatherv.c -o allgatherv.x
$> mpirun -np 4 ./allgatherv.x 
$> mpirun -np 8 ./allgatherv.x  
```

Can you explain why we use `MPI_Allgather` at line 22? 
What does the `MPI_Allreduce` do at line 47?

Importantly, how do the arrays `counts_recv` and `displacements` work?

Finally, in `all2allv2d.c` we consider the transposition of a distributed matrix.

Suppose we have a `N x M` matrix that is so big that cannot fit into the RAM of a compute node.

Importantly, in HPC matrices are stored in memory as a single 1D array
`A[i,j]=A[i*M+j]`.

We can split the matrix among `P` processes so that each process `p` have `N/P` rows.

That is every task has a local matrix `N/P x M`.

We want to transpose such distributed matrix, for this we need to perform communication.

The useful communication pattern is the `MPI_Alltoall` (or `MPI_Alltoallv` if `N` is not exactly divisible by `P`).

Such communication is performed by creating suitable buffers storing the elements to be communicated. 

After the `MPI_Alltoall`, each process need to reorganize data in memory to ensure the correct result.

 
