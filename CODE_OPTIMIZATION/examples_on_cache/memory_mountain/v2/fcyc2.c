/* Compute time used by a function f that takes two integer args */
#include <stdlib.h>
#include <sys/times.h>
#include <stdio.h>
#include <time.h>

#include "fcyc2.h"

#define CPU_TIME ({struct timespec ts; clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ), \
					 (double)ts.tv_sec + (double)ts.tv_nsec * 1e-9;})

static double *values = NULL;
int samplecount = 0;

#define KEEP_VALS 1
#define KEEP_SAMPLES 1

#if KEEP_SAMPLES
double *samples = NULL;
#endif


static void init_sampler(int k, int maxsamples)
{
  if (values)
    free(values);
  values = calloc(k, sizeof(double));
#if KEEP_SAMPLES
  if (samples)
    free(samples);
  /* Allocate extra for wraparound analysis */
  samples = calloc(maxsamples+k, sizeof(double));
#endif
  samplecount = 0;
}


/* Add new sample.  */
void add_sample(double val, int k)
{
  int pos = 0;
  if (samplecount < k) {
    pos = samplecount;
    values[pos] = val;
  } else if (val < values[k-1]) {
    pos = k-1;
    values[pos] = val;
  }
#if KEEP_SAMPLES
  samples[samplecount] = val;
#endif
  samplecount++;
  /* Insertion sort */
  while (pos > 0 && values[pos-1] > values[pos]) {
    double temp = values[pos-1];
    values[pos-1] = values[pos];
    values[pos] = temp;
    pos--;
  }
}

/* Get current minimum */
double get_min()
{
  return values[0];
}

/* What is relative error for kth smallest sample */
double err(int k)
{
  if (samplecount < k)
    return 1000.0;
  return (values[k-1] - values[0])/values[0];
}

/* Have k minimum measurements converged within epsilon? */
int has_converged(int k_arg, double epsilon_arg, int maxsamples)
{
  if ((samplecount >= k_arg) &&
      ((1 + epsilon_arg)*values[0] >= values[k_arg-1]))
    return samplecount;
  if ((samplecount >= maxsamples))
    return -1;
  return 0;
}

/* Code to clear cache */
#define ASIZE (1 << 20)
#define STRIDE 8
static int stuff[ASIZE];
static int sink;

static void clear()
{
  int x = sink;
  int i;
  for (i = 0; i < ASIZE; i += STRIDE)
    x += stuff[i];
  sink = x;
}

double fcyc2_full(test_funct f, int param1, int param2, int clear_cache,
		 int k, double epsilon, int maxsamples, int compensate) 
{
  double result;
  init_sampler(k, maxsamples);
  if (compensate) {
    do {
      if (clear_cache)
	clear();
      f(param1, param2);   /* warm cache */
      double tstart = CPU_TIME;
      f(param1, param2);
      tstart = CPU_TIME - tstart;
      add_sample(tstart, k);
    } while (!has_converged(k, epsilon, maxsamples) && samplecount < maxsamples);
  } else {
    do {
      if (clear_cache)
	clear();
      f(param1, param2); /* warm cache */
      double tstart = CPU_TIME;
      f(param1, param2);
      tstart = CPU_TIME-tstart;
      add_sample(tstart, k);
    } while (!has_converged(k, epsilon, maxsamples) && samplecount < maxsamples);
  }
#ifdef DEBUG
  {
    int i;
    printf(" %d smallest values: [", k);
    for (i = 0; i < k; i++)
      printf("%.0f%s", values[i], i==k-1 ? "]\n" : ", ");
  }
#endif
  result = values[0];
#if !KEEP_VALS
  free(values); 
  values = NULL;
#endif
  return result;  
}


double fcyc2(test_funct f, int param1, int param2, int clear_cache)
{
  return fcyc2_full(f, param1, param2, clear_cache, 3, 0.01, 500, 0);
}







