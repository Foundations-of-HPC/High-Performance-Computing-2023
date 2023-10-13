module purge
module load openMPI/4.1.5/gnu
mpicc scatter.c -o scatter_c.x
mpicc gather.c  -o gather_c.x
mpicc b_cast.c  -o b_cast_c.x
mpicc reduce.c  -o reduce_c.x
mpicc mpi_bcastcompare.c -o mpi_bcastcompare.x
mpicc allgatherv.c -o allgatherv.x
mpicc all2allv3d.c -o all2allv3d.x

mpifort scatter.f -o scatter_f.x
mpifort scatter.f -o scatter_f.x
mpifort gather.f  -o gather_f.x
mpifort b_cast.f  -o b_cast_f.x
mpifort reduce.f  -o reduce_f.x
