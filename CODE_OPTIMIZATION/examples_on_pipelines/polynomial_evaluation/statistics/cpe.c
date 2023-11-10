/* Compute CPE for function */
#include <stdlib.h>
#include <stdio.h>
#include "../timing/clock.h"
#include "fcyc.h"
#include "cpe.h"
#include "lsquare.h"

/* Find number of cycles taken by function.
   Do this by running number of trials until best two within TOL of
   each other
*/
double measure_function(elem_fun_t f, int cnt)
{
    /* Need to fudge fact that fcyc wants a function taking an
       int *, while our function takes an int */
    test_funct tf = (test_funct) f;
    return fcyc(tf, (int *) (long int) cnt);
}

#define MAXCNT 100

#define LIM ((unsigned) (1<<30))

/* LCM of unrolling degree */
#ifdef USE_UNI
#define UNROLL 32
#else /* USE_UNI */
#define UNROLL 1
#endif

static int get_cnt(int index, int samples,
		   int maxcnt, sample_t smethod, double bias)
{
  int mincnt = (int) (bias*maxcnt);
  double weight;
  int val;
  switch (smethod) {
  case UNI_SAMPLE:
    weight = (double) index/(samples - 1);
    break;
  case RAN_SAMPLE:
    weight = (double) ((unsigned) random() % LIM) / (double) (LIM-1);
    break;
  default:
    fprintf(stderr, "Undefined sampling method %d\n", smethod);
    exit(1);
  }
  val = mincnt + (int) (weight*(maxcnt-mincnt));
  if (val < 0) {
      fprintf(stderr, "Invalid val = %d\n", val);
      fprintf(stderr, "mincnt = %d, maxcnt = %d, weight = %f\n", mincnt, maxcnt, weight);
      fprintf(stderr, "maxcnt-mincnt = %d\n", maxcnt-mincnt);
      fprintf(stderr, "(int) (weight*(maxcnt-mincnt)) = %d\n",
	      (int) (weight*(maxcnt-mincnt)));
      exit(1);
  }
  return UNROLL * (val/UNROLL);

}

#define SEED 31415

/* Find cpe for function f, which allows cnt up to maxcnt, using
   specified number of sample points.
   If data_file, then print data so that can plot points with Excel
   smethod determines method for generating samples
*/
double find_cpe_full(elem_fun_t f, int maxcnt, int samples, FILE *data_file,
		     sample_t smethod, double bias, int verbose)
{
  int i;
  int cnt;
  double cpe;
  double overhead = 0;
  double *cnt_val = calloc(samples, sizeof(double));
  double *cycle_val = calloc(samples, sizeof(double));
  /* Do the samples */

  srandom(SEED);
  start_counter_copy();
  for (i = 0; i < samples; i++) {
    cnt = get_cnt(i, samples, maxcnt, smethod, bias);
    cnt_val[i] = cnt;
    cycle_val[i] = measure_function(f, cnt);
  }
  double run_time = get_counter_copy();
  printf("\trun-time cycles: %g\n", run_time);
  /* Fit data */
  cpe = ls_slope(cnt_val, cycle_val, samples);
  if (data_file)
    overhead = ls_intercept(cnt_val, cycle_val, samples);
  if (data_file && verbose) {
    /* Print x values */
    fprintf(data_file, "Cnt\t0");
    for (i = 0; i < samples; i++) 
      fprintf(data_file, "\t%.0f", cnt_val[i]);
    fprintf(data_file, "\n");
    /* Print y values */
    fprintf(data_file, "Cycs.\t");
    for (i = 0; i < samples; i++) 
      fprintf(data_file, "\t%.2f", cycle_val[i]);
    fprintf(data_file, "\n");
#if 0
    /* Print (y-b)/x values */
    fprintf(data_file, "CPE");
    for (i = 0; i < samples; i++) 
      fprintf(data_file, "\t%.2f", (cycle_val[i]-overhead)/cnt_val[i]);
    fprintf(data_file, "\n");
#endif
    /* Print ax*b values */
    fprintf(data_file, "Interp.\t%.2f", overhead);
    for (i = 0; i < samples; i++) 
      fprintf(data_file, "\t%.2f", cpe*cnt_val[i]+overhead);
    fprintf(data_file, "\n");
  }
  if (data_file && verbose) {
    /* Print results */
    fprintf(data_file, "cpe\t%.2f\tovhd\t%.2f\tavgerr\t\%.3f\tmaxerr\t\%.3f\n",
	    cpe, overhead,
	    ls_error(cnt_val, cycle_val, samples, LS_AVG),
	    ls_error(cnt_val, cycle_val, samples, LS_MAX));
  }
  free(cnt_val);
  free(cycle_val);
  return cpe;
}

/* Use default parameters */
double find_cpe(elem_fun_t f, int maxcnt)
{
  return find_cpe_full(f, maxcnt, 100, stdout, RAN_SAMPLE, 0.3, 0);
}







