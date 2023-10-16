module purge
module load openMPI/4.1.5/icx

mpicc  Brecv.c                -g3 -o Brecv.x
mpicc  CBlockSends.c          -g3 -o CBlockSends.x
mpicc  deadlock.c             -g3 -o deadlock.x
mpicc  linear-array.c         -g3 -o linear-array.x
mpicc  mpi_env_call.c         -g3 -o mpi_env_call.x
mpicc  mpi_hello_world.c      -g3 -o mpi_hello_world.x
mpicc  mpi_hello_world_sync.c -g3 -o mpi_hello_world_sync.x
mpif90 mpi_hello_world.F90    -g3 -o mpi_hello_world_F.x
mpicc  mpi_pi.c           -O3 -g3 -o mpi_pi.x
mpif90 send_message.F90       -g3 -o send_message_F.x
mpicc  send_message.c         -g3 -o send_message.x
mpicc  sendrecv_message.c     -g3 -o sendrecv_message.x
