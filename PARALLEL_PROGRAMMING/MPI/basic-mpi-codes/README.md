# basic-mpi-codes

Let's allocate 8 tasks on one EPYC node in interactive way by means of
  
`$> salloc -n8 -N1 -p EPYC --time=2:00:0 --mem=10GB`

Then load openMPI using

`$> module load openMPI/4.1.5/gnu`

Check that the module is properly load usign

`$> module list`

Try to compile mpi_env_call.c by means of

`$> mpicc mpi_env_call.c -o mpi_env_call.x`

Run using just 4 tasks with

`$> mpirun -np 4 ./mpi_env_call.x`

Run using 8 tasks with

`$> mpirun -np 8 ./mpi_env_call.x`

Can you understand what does the code do?

Try with 9 tasks:

`$> mpirun -np 9 ./mpi_env_call.x`

What do you notice? Why?

Similarly, compile and run mpi_hello_world.c

```
$> mpicc mpi_hello_world.c -o mpi_hello_world.x
$> mpirun -np 8 ./mpi_hello_world.x
```

Are the ranks sorted?

Now, try 

```
$> mpicc mpi_hello_world_sync.c -o mpi_hello_world_sync.x
$> mpirun -np 8 ./mpi_hello_world_sync.x 
```

Are the ranks sorted? Why? How we could synchronize the processes?

Let's try to send and receive some messages

```
$> mpicc send_message.c -o send_message.x
$> mpirun -np 4 ./send_message.x  
```

What do you notice? Why?

Try:

`$> mpirun -np 2 ./send_message.x`

Now, modify line 24:

`buffer = 33;`

with

`buffer = 34;`

Compile and run:

```
$> mpicc send_message.c -o send_message.x
$> mpirun -np 2 ./send_message.x
```

What happens? Why?

Restore to `buffer = 33;` (you can use: `$> git checkout send_message.c`)

Modify line 19 

`MPI_Recv(&buffer, 1, MPI_INT, 1, 9, MPI_COMM_WORLD, &status);`

with 

`MPI_Recv(&buffer, 1, MPI_INT, 1, 10, MPI_COMM_WORLD, &status);`

Compile and run:

```
$> mpicc send_message.c -o send_message.x
$> mpirun -np 2 ./send_message.x 
```

What happens? Why? (If you get stuck use ^C)

Modify also line 27

`MPI_Send(&buffer, 1, MPI_INT, 0, 9, MPI_COMM_WORLD);`

with 

`MPI_Send(&buffer, 1, MPI_INT, 0, 10, MPI_COMM_WORLD);`

Compile and run:

```
$> mpicc send_message.c -o send_message.x
$> mpirun -np 2 ./send_message.x
```

What happens? Why?

Compile and run sendrecv_message.c

```
$> mpicc sendrecv_message.c -o sendrecv_message.x 
$> mpirun -np 2 ./sendrecv_message.x
```

Modify the lines 25 and 33 with

```
MPI_Sendrecv(message, 21, MPI_CHAR, 1, 10, &buffer,  1, MPI_INT,  1,  9,
             MPI_COMM_WORLD, &status);

MPI_Sendrecv(&buffer,  1,  MPI_INT, 0,  9, message, 21, MPI_CHAR, 0, 10,
             MPI_COMM_WORLD, &status);

```

Compile and run sendrecv_message.c

```
$> mpicc sendrecv_message.c -o sendrecv_message.x
$> mpirun -np 2 ./sendrecv_message.x
```

What do you notice? Why?

Compile and run 

```
$> mpicc linear-array.c -o linear-array.x
$> mpirun -np 2 ./linear-array.x
$> mpirun -np 4 ./linear-array.x
$> mpirun -np 8 ./linear-array.x
```

What are the outputs? Why?

Now, compile and run 

```
$> mpicc mpi_pi.c -o mpi_pi.x
$> mpirun -np 2 ./mpi_pi.x
```

What happens? Why?

Now, try 

`$> mpirun -np 8 ./mpi_pi.x 1000000`

Why we have `srand48(SEED * (myid + 1));` ?

What is the purpose of MPI_Reduce in this program?

Compile and run 

```
$> mpicc Brecv.c -o Brecv.x
$> mpirun -np 2 ./Brecv.x 10
$> mpirun -np 2 ./Brecv.x 100
$> mpirun -np 2 ./Brecv.x 1000
$> mpirun -np 2 ./Brecv.x 10000
$> mpirun -np 2 ./Brecv.x 100000
$> mpirun -np 2 ./Brecv.x 1000000
```

What do you observe in terms of latency and bandwidth?

Compile and run

```
$> mpicc CBlockSends.c -o CBlockSends.x
$> mpirun -np 2 ./CBlockSends.x 10
$> mpirun -np 2 ./CBlockSends.x 100
$> mpirun -np 2 ./CBlockSends.x 1000
$> mpirun -np 2 ./CBlockSends.x 10000
$> mpirun -np 2 ./CBlockSends.x 100000
$> mpirun -np 2 ./CBlockSends.x 1000000
```

Which send is the fastest?

Compile and run

`$> mpicc deadlock.c -o deadlock.x `

`$> mpirun -np 2 ./deadlock.x `

Why does it work?

Replace line 52

`MPI_Send(&rmsg1, MSGLEN, MPI_FLOAT, idest, istag, MPI_COMM_WORLD);`

with 

`MPI_Ssend(&rmsg1, MSGLEN, MPI_FLOAT, idest, istag, MPI_COMM_WORLD);`

What happens? Why?

