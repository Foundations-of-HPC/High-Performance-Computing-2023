      PROGRAM reduce
      INCLUDE 'mpif.h'
      INTEGER ierr, myid, nproc, root
      INTEGER status(MPI_STATUS_SIZE)
      REAL A(2), res(2)
      CALL MPI_INIT(ierr)
      CALL MPI_COMM_SIZE(MPI_COMM_WORLD, nproc, ierr)
      CALL MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierr)
      root = 0
      a(1) = 2.0
      a(2) = 4.0
      CALL MPI_REDUCE(a, res, 2, MPI_REAL, MPI_SUM, root,
     &  MPI_COMM_WORLD, ierr)
      IF( myid .EQ. 0 ) THEN
        WRITE(6,*) myid, ': res(1)=', res(1), 'res(2)=', res(2)
      END IF
      CALL MPI_FINALIZE(ierr)
      END
