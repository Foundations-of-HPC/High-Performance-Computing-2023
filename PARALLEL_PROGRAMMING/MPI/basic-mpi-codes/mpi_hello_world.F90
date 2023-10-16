PROGRAM hello
 INCLUDE 'mpif.h'
 INTEGER err, rank, size, name_len
 CHARACTER(MPI_MAX_PROCESSOR_NAME) processor_name
 CALL MPI_INIT(err)
 CALL MPI_COMM_RANK(MPI_COMM_WORLD,rank,err)
 CALL MPI_COMM_SIZE(MPI_COMM_WORLD,size,err)
 CALL MPI_GET_PROCESSOR_NAME(processor_name,name_len,err)
 print *, 'Hello world from processor ', processor_name, ' rank ', rank, ' out of ', size, ' processors'
 CALL MPI_FINALIZE(err)
END
