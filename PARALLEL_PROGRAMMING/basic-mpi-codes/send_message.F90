
Program MPI 
! a simple implementation of send/receive message 
   Implicit None
!
   Include 'mpif.h'
!
   Integer                                 :: rank
   Integer                                 :: buffer
   Integer, Dimension( 1:MPI_status_size ) :: status
   Integer                                 :: error
!
   Call MPI_init( error )
   Call MPI_comm_rank( MPI_comm_world, rank, error )
!
   If( rank == 0 ) Then
      Call MPI_recv( buffer, 1, MPI_integer, 1, 10, &
                     MPI_comm_world, status, error )
      Print*, 'Rank ', rank, ' buffer=', buffer
      If( buffer /= 33 ) Print*, 'fail'
   End If                      
!
   If( rank == 1 ) Then
      buffer = 33
      Call MPI_send( buffer, 1, MPI_integer, 0, 10, &
                     MPI_comm_world, error )
   End If
!
   Call MPI_finalize( error )
End Program MPI
