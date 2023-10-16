        PROGRAM broad_cast
        INCLUDE 'mpif.h'
        INTEGER ierr, myid, nproc, root
        INTEGER status(MPI_STATUS_SIZE)
        REAL A(2)
        CALL MPI_INIT(ierr)
        CALL MPI_COMM_SIZE(MPI_COMM_WORLD, nproc, ierr)
        CALL MPI_COMM_RANK(MPI_COMM_WORLD, myid, ierr)
        root = 0
        a(1)=0.0
        A(2)=0.0
        IF( myid .EQ. 0 ) THEN
        a(1) = 2.0
        a(2) = 4.0
        END IF
        WRITE(6,*) myid, ' before: a(1)=', a(1), 'a(2)=', a(2)
        CALL MPI_BARRIER() 
        CALL MPI_BCAST(a, 2, MPI_REAL, 0, MPI_COMM_WORLD, ierr)
        WRITE(6,*) myid, ' after : a(1)=', a(1), 'a(2)=', a(2)
        CALL MPI_FINALIZE(ierr)
        END
