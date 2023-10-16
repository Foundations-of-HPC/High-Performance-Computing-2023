       PROGRAM scatter
        INCLUDE 'mpif.h'
        INTEGER ierr, myid, nproc, nsnd, I, root
        INTEGER status(MPI_STATUS_SIZE)
        REAL A(16), B(2)
        CALL MPI_INIT(ierr)
        CALL MPI_COMM_SIZE(MPI_COMM_WORLD, nproc, ierr)
        CALL MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierr)
        root = 0
        IF( myid .eq. root ) THEN
          DO i = 1, 16
             a(i) = REAL(i)
          END DO
        END IF
        nsnd = 2
        CALL MPI_SCATTER(a, nsnd, MPI_REAL, b, nsnd,
     & MPI_REAL, root, MPI_COMM_WORLD, ierr)
        WRITE(6,*) myid, ': b(1)=', b(1), 'b(2)=', b(2)
        CALL MPI_FINALIZE(ierr)
       END
