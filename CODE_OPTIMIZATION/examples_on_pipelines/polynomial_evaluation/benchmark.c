#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "statistics/cpe.h"
#include "poly.h"

#define SHORT 0
#if SHORT 
#define ASIZE 31
#else
#define ASIZE 973
#endif
#define EPS (1e-8)

/* Keep track of a number of different programs */
#define MAX_BENCHMARKS 100

static struct {
    poly_t cfunct;
    char *description;
    double cpe;
} benchmarks[MAX_BENCHMARKS];

static int benchmark_count = 0;
static int current_benchmark = 0;

static double* data = NULL;
static double x;
static double result;
static poly_t check_func = NULL;

static void setup()
{
	int i;
	if (!data)
		data = (double*) malloc(sizeof(double) * ASIZE);
	if (!data) {
		fprintf(stderr, "Memory allocation error!\n");
		exit(EXIT_FAILURE);
	}
	/* Initialize array  */
	for (i = 0; i < ASIZE; i++)
		data[i] = (drand48() * 2) - 1;
	x = (drand48() * 2) - 1;
}

void run(int cnt) {
	result = benchmarks[current_benchmark].cfunct(data, x, cnt);
}

static void run_test(int bench_index) {
	double cpe;
	char *description = benchmarks[bench_index].description;
	double good_result;
	current_benchmark = bench_index;
	printf("starting benchmark %d\n", bench_index);
	setup();
	cpe = find_cpe_full(run, ASIZE, 200000, stdout, RAN_SAMPLE, 0.3, 0);
	if (check_func) {
		result = benchmarks[bench_index].cfunct(data, x, ASIZE);
		good_result = check_func(data, x, ASIZE);
		if (result - good_result > EPS) {
			printf("Function %s, Should be %f, Got %f\n",
			       description, good_result, result);
		}
	}
	benchmarks[current_benchmark].cpe = cpe;
	/* print results */
	printf("%s: ", description);
	printf("%.2f cycles/element\n\n", cpe);
}

void add_function(poly_t f, char *description) {
	benchmarks[benchmark_count].cfunct = f;
	benchmarks[benchmark_count].description = description;
	benchmark_count++;
}

void set_check_function(poly_t f) {
	check_func = f;
}

int main()
{
	int i;
	register_functions();
	printf("\n");
	for (i = 0; i < benchmark_count; i++) {
		run_test(i);
	}
	free(data);
	return EXIT_SUCCESS;
}

