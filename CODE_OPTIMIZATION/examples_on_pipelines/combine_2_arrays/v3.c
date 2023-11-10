//
//
//
{
  int N_4 = (N/4)*4;
      
#pragma ivdep
  for ( int i = 0; i < N_4; i+=4 )
    {
      sum += array1[i] * array2[i] +
	array1[i+1] * array2[i+1] +
	array1[i+2] * array2[i+2] +
	array1[i+3] * array2[i+3];
    }

  for ( int i = N_4; i < N; i++ )
    sum += array1[i] * array2[i];     
}
