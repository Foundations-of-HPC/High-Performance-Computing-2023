{

#define _DO_NOT_OPTIMIZE_BEGIN __asm__ volatile("" ::: "memory");
#define _DO_NOT_OPTIMIZE_END __asm__ volatile("" ::: "memory");
  
  typedef double v4df __attribute__ ((vector_size (4*sizeof(double))));
  typedef union {
    v4df   V;
    double v[4];
  }v4df_u;


#ifdef _GNU_SOURCE      
  v4df sum_ = {0, 0, 0, 0};
#else
  v4df sum_ = {0};
#endif
  v4df register mytmp;
  v4df register tmp  = *((v4df*)&array1[0]) * *((v4df*)&array2[0]);

  int N__4 = N/4;
  int N_4 = N__4*4;

#pragma ivdep
  for( int i = 1; i <= N__4; i++)
    {
      _DO_NOT_OPTIMIZE_BEGIN;
      mytmp = *((v4df*)array1 + i) * *((v4df*)array2 + i);
      _DO_NOT_OPTIMIZE_END;
      sum_ += tmp;
      tmp   = mytmp;
    }

  for ( int i = N_4; i < N; i++ )
    sum += array1[ i ] * array2[ i ];           
      
  v4df_u sum_u ;
      
  sum_u.V = sum_;
      
  sum_u.v[0] = sum_u.v[0] + sum_u.v[1];
  sum_u.v[2] = sum_u.v[2] + sum_u.v[3];
      
  sum += sum_u.v[0] + sum_u.v[2];
      
}
