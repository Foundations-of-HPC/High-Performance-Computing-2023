        PROGRAM gather
        INCLUDE 'mpif.h'
        INTEGER ierr, myid, nproc, nsnd, I, root
        INTEGER status(MPI_STATUS_SIZE)
        REAL A(16), B(2)
        CALL MPI_INIT(ierr)
        CALL MPI_COMM_SIZE(MPI_COMM_WORLD, nproc, ierr)
        CALL MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierr)
        root = 0
        b(1) = REAL( myid )
        b(2) = REAL( myid )
        nsnd = 2
        CALL MPI_GATHER(b, nsnd, MPI_REAL, a, nsnd,
     & MPI_REAL, root, MPI_COMM_WORLD, ierr)
        IF( myid .eq. root ) THEN
                DO i = 1, (nsnd*nproc)
                 WRITE(6,*) myid, ': a(i)=', a(i)
                END DO
        END IF
        CALL MPI_FINALIZE(ierr)
        END
