#  Compiling and Running MPI program on ORFEO

---
Author: Niccolo Tosato
---

## How to *compile* and *run* MPI hello world

The focus now is not in MPI programming, but in how *compile* and *run* it in a cluster with SLURM. 

Simple hello world:

```
// Source code from https://mpitutorial.com/tutorials/mpi-hello-world/

#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    // Initialize the MPI environment
    MPI_Init(NULL, NULL);

    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d out of %d processors\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();
}
```

### Compile:

#### Preliminaries

**Important step**: First of all allocate a single core on the machine that will be used to run the program. Usually a program must be compiled in a machine with the **same** architecture of the machine that will run the code (there are other ways but this is the easier one). Most *HPC* libraries and software will detect the hardware at compile time.

Allocate an *epyc* machine or a *thin* machine:

`salloc -n1 -N1 -p FHPC --time=0:15:00`

and load the desired *profile* with the module system.

*Remember*: If an epyc node is requested, use *AMD* profile, in case of a *thin* node use *Intel* profile. 

`module load architecture/AMD` or `module load architecture/Intel`

In case of doubts check the cpu architecture using `srun lscpu`. SLURM will execute `lscpu` in the allocated machine and send back the output. 

After loading the architecture profile is necessary to choose an MPI implementation. All version are visible with `module avail` under section `mpi`. The two main implementations available in Orfeo are *openMPI* and *intel MPI*. 
 
To load openMPI use: `module load openMPI/4.1.4/gnu/12.2.1`

Finally compile using `srun mpicc MPI_hello_world.c -o hello_world.x`.

Why are we calling `srun` before `mpicc`? Because in this way the compilation is executed on the target machine and the compiler could use all possible optimizations for the specific architecture (remember `--march=native`).

Another option is to open a console in the target machine using:
`srun -n4 -N1 -p FHPC --time=0:15:00 --pty bash`.

The user will be forwarded to the machine and is not necessary anymore to prepend `srun` before the build command.

```
[ntosato@login02 ~]$ srun -N1 -n4 -p FHPC --time=0:15:00 --pty bash
[ntosato@epyc004 ~]$ mpicc MPI_hello_world.c -o hello_world.x
```
*Note*: look **always** at the host name.

#### `mpicc` 

To compile MPI programs you will use `mpicc` but that is a wrapper and not a compiler. `mpicc` uses the local C compiler with some specific flags and paths.

To inspect what happens under the hood (in case of openMPI) add the `-showme` flag:


```
[ntosato@epyc004 tutorial]$ mpicc -showme MPI_hello_world.c -o hello_world.x 
gcc MPI_hello_world.c -o hello_world.x 
-I/opt/area/shared/programs/AMD/openMPI/4.1.4/include 
-L/opt/area/shared/programs/AMD/openMPI/4.1.4/lib
 -Wl,-rpath -Wl,/opt/area/shared/programs/AMD/openMPI/4.1.4/lib -Wl,--enable-new-dtags -lmpi
```
 
 It's clear that `gcc` is used as the compiler and `mpicc` sets the correct paths to the headers and libraries.

To compile the program manually we can use the environment variables loaded from the MPI module:

 ```
 [ntosato@epyc004 tutorial]$ gcc MPI_hello_world.c -o hello_world.x -L $MPI_LIB -I $MPI_INCLUDE -lmpi
 ```

Be aware that this method is discouraged and error prone: don't use it.

### Run

Once the compile phase is completed, release the requested resources and allocate the resources to run the executable. 

```
[ntosato@login02 ~]$ salloc -N3 --ntasks-per-node 3 -p FHPC --time=0:10:00
[ntosato@login02 ~]$ mpirun hello_world.x 
Hello world from processor epyc006, rank 6 out of 9 processors
Hello world from processor epyc004, rank 2 out of 9 processors
Hello world from processor epyc005, rank 4 out of 9 processors
Hello world from processor epyc006, rank 7 out of 9 processors
Hello world from processor epyc004, rank 0 out of 9 processors
Hello world from processor epyc004, rank 1 out of 9 processors
Hello world from processor epyc005, rank 3 out of 9 processors
Hello world from processor epyc006, rank 8 out of 9 processors
Hello world from processor epyc005, rank 5 out of 9 processors

```

By default `mpirun` uses all allocated processors.

To check the correctness of the hostname, try to run `srun hostname`, this must match with the one provided by `hello_world.x`.

