//
//
//
{
      int N_4 = (N/4)*4;
      double A[2];
      int idx = 0;
      
      A[0] = array1[0] * array2[0];
      for ( int i = 0; i < N_4; i+=4 )
	{
	  int noidx = !idx;
	  
	  A[noidx] = array1[i+4] * array2[i+4];
	  
	  sum += A[idx] +
	    array1[i+1] * array2[i+1] +
	    array1[i+2] * array2[i+2] +
	    array1[i+3] * array2[i+3];

	  idx = noidx;

	}

      for ( int i = N_4; i < N; i++ )
	sum += array1[i] * array2[i];
}
