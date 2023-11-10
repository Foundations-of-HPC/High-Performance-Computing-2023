/*
 * authors   : Giuseppe Piero Brandino - eXact-lab s.r.l.
 * date      : October 2018
 * copyright : GNU Public License
 */

#include <stdio.h>
#include <sys/time.h>
#include <stdlib.h>

double cclock()
  /* Returns elepsed seconds past from the last call to timer rest */
{

    struct timeval tmp;
    double sec;
    gettimeofday( &tmp, (struct timezone *)0 );
    sec = tmp.tv_sec + ((double)tmp.tv_usec)/1000000.0;
    return sec;
}

void setup_matrix(double* a, int n, int m, int stride)
{
    int i,j;
    for (i = 0; i < n; i++)
        for (j = 0; j < m; j++)
            a[i*m + j ] = i*m + j + stride;

}

void clear_matrix(double* a, int n, int m)
{
    int i,j;
    for (i = 0; i < n; i++)      
        for (j = 0; j < m; j++)
            a[i*m + j ] = 0;

}


void mat_mult(double* a, double* b, double* c, int n, int m, int o)
{

    int i, j, k;
    for (i = 0; i < n; i++)
        for (j = 0; j < o; j++)
            for ( k = 0; k < m; k++)
                c[i*o + j] += a[i*m + k] * b[k*o + j];
}


void mat_mult_opt(double* a, double* b, double* c, int n, int m, int o)
{

    int i, j, k;
    for (i = 0; i < n; i++)
        for (k = 0; k < m; k++)
            for (j = 0; j < o; j++)
                c[i*o + j] += a[i*m + k] * b[k*o + j];


}


int main(int argc, char** argv)
{

   double *a, *b, *c;
   int w, m,n,o;
   double begin, end;
     
   if (argc < 5)
     {
       printf(" Calculates c(n,o)=a(n,m)*b(m,o) \n");
       printf(" Usage: %s case n m o ", argv[0]);
       return 1;
     }

   w=atoi(argv[1]);
   n=atoi(argv[2]);
   m=atoi(argv[3]);
   o=atoi(argv[4]);

   a = malloc(n * m * sizeof(double));
   b = malloc(m * o * sizeof(double));
   c = malloc(n * o * sizeof(double));

   setup_matrix(a, n, m, 0);
   setup_matrix(b, m, o, m*n);
   clear_matrix(c, n, o);

   if( w == 0 )
     {
       begin = cclock();
       mat_mult(a, b, c, n, m, o);
       end = cclock();
       printf ("NON-optimized elapsed time %9.4f s  \n\n", end - begin );
     }
   else
     {
       begin = cclock();
       mat_mult_opt(a, b, c, n, m, o);
       end = cclock();
       printf ("    Optimized Elapsed time %9.4f s  \n\n", end - begin );
     }

//   printf("%f\n", c[0]);
   free(a);
   free(b); 
   free(c);

   return 0;

}
