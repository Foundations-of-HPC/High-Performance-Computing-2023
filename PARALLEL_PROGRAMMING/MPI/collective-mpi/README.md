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

In `gather.c` we gather on root (rank 0) arrays of a given (fixed) number of elements.

In `allgatherv.c`, instead of gathering the arrays on root, we allgather on all ranks. 
That is every rank will gather the arrays. 

Moreover, we gather arrays with a different numbers of elements (chosen randomly).

Before Allgatherv

`P0`:
 
|  0 |  0 |  0 | 
|----|----|----|


 `P1`:
 
| 1  | 1  | 1  | 1  |  1 |  
|----|----|----|----|----|  

 `P2`:
 
| 2  | 2  | 2  | 2  |  
|----|----|----|----|

 `P3`:
 
| 3  | 3  | 3  | 3  | 3  | 3  | 3  |  
|----|----|----|----|----|----|----| 

After Allgatherv:

`P0`:
 
|  0 |  0 |  0 | 1  | 1  | 1  | 1  |  1 | 2  | 2  | 2  | 2  | 3  | 3  | 3  | 3  | 3  | 3  | 3  |    
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|


 `P1`:

|  0 |  0 |  0 | 1  | 1  | 1  | 1  |  1 | 2  | 2  | 2  | 2  | 3  | 3  | 3  | 3  | 3  | 3  | 3  |    
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|

 `P2`:
 
|  0 |  0 |  0 | 1  | 1  | 1  | 1  |  1 | 2  | 2  | 2  | 2  | 3  | 3  | 3  | 3  | 3  | 3  | 3  |    
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|

 `P3`:
 
|  0 |  0 |  0 | 1  | 1  | 1  | 1  |  1 | 2  | 2  | 2  | 2  | 3  | 3  | 3  | 3  | 3  | 3  | 3  |    
|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|----|

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

Original Matrix:
 
 `P0`:
 
|  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8|
|----|----|----|----|----|----|----|----|
|  9 | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 

 `P1`:
 
| 17 | 18 | 19 | 20 | 21 | 22 | 23 | 24 |
|----|----|----|----|----|----|----|----|
| 25 | 26 | 27 | 28 | 29 | 30 | 31 | 32 | 

 `P2`:
 
| 33 | 34 | 35 | 36 | 37 | 38 | 39 | 40 |
|----|----|----|----|----|----|----|----|
| 41 | 42 | 43 | 44 | 45 | 46 | 47 | 48 | 

 `P3`:
 
| 49 | 50 | 51 | 52 | 53 | 54 | 55 | 56 |
|----|----|----|----|----|----|----|----|
| 57 | 58 | 59 | 60 | 61 | 62 | 63 | 64 |                                                                                                                     


Buffer Send:

 `P0`:
 
|  1 |  2 |  
|----|----| 
|  9 | 10 |  
|  3 |  4 |
| 11 | 12 |
|  5 |  6 |
| 13 | 14 |
|  7 |  8 |
| 15 | 16 | 

 `P1`:
 
| 17 | 18 |  
|----|----| 
| 25 | 26 |  
| 19 | 20 |
| 27 | 28 |
| 21 | 22 | 
| 29 | 30 |
| 23 | 24 |
| 31 | 32 | 

 `P2`:
 
| 33 | 34 |  
|----|----| 
| 41 | 42 |  
| 35 | 36 |
| 43 | 44 | 
| 37 | 38 |
| 45 | 46 |
| 39 | 40 |
| 47 | 48 | 

`P3`:
 
| 49 | 50 |  
|----|----| 
| 57 | 58 |  
| 51 | 52 | 
| 59 | 60 | 
| 53 | 54 |
| 61 | 62 | 
| 55 | 56 |
| 63 | 64 |  


After Alltoall:

Buffer Receive:

 `P0`:
 
|  1 |  2 |  
|----|----| 
|  9 | 10 |  
| 17 | 18 |  
| 25 | 26 | 
| 33 | 34 |  
| 41 | 42 |  
| 49 | 50 |  
| 57 | 58 | 

 `P1`:
 
|  3 |  4 |
|----|----| 
| 11 | 12 |
| 19 | 20 |
| 27 | 28 |
| 35 | 36 |
| 43 | 44 | 
| 51 | 52 | 
| 59 | 60 | 

 `P2`:
 
|  5 |  6 |
|----|----| 
| 13 | 14 |
| 21 | 22 | 
| 29 | 30 |
| 37 | 38 |
| 45 | 46 |
| 53 | 54 |
| 61 | 62 | 

`P3`:
 
|  7 |  8 |
|----|----| 
| 15 | 16 |  
| 23 | 24 |
| 31 | 32 | 
| 39 | 40 |
| 47 | 48 | 
| 55 | 56 |
| 63 | 64 |  


After data reorganization:

Transposed Matrix:
 
 `P0`:
 
|  1 |  9 | 17 | 25 | 33 | 41 | 49 | 57 | 
|----|----|----|----|----|----|----|----|     
|  2 | 10 | 18 | 26 | 34 | 42 | 50 | 58 |   

 `P1`:
 
|  3 | 11 | 19 | 27 | 35 | 43 | 51 | 59 |
|----|----|----|----|----|----|----|----| 
|  4 | 12 | 20 | 28 | 36 | 44 | 52 | 60 |


 `P2`:
 
|  5 | 13 | 21 | 29 | 37 | 45 | 53 | 61 |
|----|----|----|----|----|----|----|----|
|  6 | 14 | 22 | 30 | 38 | 46 | 54 | 62 | 

`P3`:
 
|  7 | 15 | 23 | 31 | 39 | 47 | 55 | 63 | 
|----|----|----|----|----|----|----|----| 
|  8 | 16 | 24 | 32 | 40 | 48 | 56 | 64 | 


 
