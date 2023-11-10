//
//
//
{
  double sum1 = 0, sum2 = 0;
  /* double * const array1_ref = array1; */
  /* double * const array2_ref = array2; */

#pragma ivdep
  for ( int i = 0; i < N-1; i+=2 )
    // simply unrolling 2 times, exposes the fact that at least
    // 2 elements of the array can be processed independently
    {
      sum1 += array1[   i ] * array2[   i ];
      sum2 += array1[ i+1 ] * array2[ i+1 ];
    }
  if ( N % 2 )
    sum = array1[ N-1 ] * array2[ N-1 ];

  sum += sum1 + sum2;
}
