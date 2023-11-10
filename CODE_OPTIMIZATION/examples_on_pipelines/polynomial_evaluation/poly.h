#if __INTEL_COMPILER
/* inline function definitions */
#pragma warning ( disable : 1418 )
#endif

typedef double (*poly_t)(double*, double, int);
/* Add routine to list of programs to measure */
void add_function(poly_t f, char *description);
/* Set routine to check results against */
void set_check_function(poly_t f);
/* called by main to register the set of routines to benchmark */
void register_functions(void);

