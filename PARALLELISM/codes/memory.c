#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void callocator(double ** vec, size_t N)
{
  *vec=(double *)calloc(N, sizeof(double));
  if (*vec==NULL) fprintf(stderr, "vec is NULL\n");
}

int main(int argc, char **argv)
{
  double * v ;
  size_t i, j, m;
  for (i = 1e3 ; i < 1e8 ; i*=10 ) {
    m = sizeof(double) * i ;
    callocator(&v, i);
    for (j=0; j<i; j++) {
      v[j] = 1.; 
    }
    free(v);
    fprintf(stdout, "mem = %llu B \n", m);
    fflush(stdout);
    sleep(30);
    fprintf(stdout, "We have waited 30 seconds\n", m);
    fflush(stdout);
  }
  return 0;
}
