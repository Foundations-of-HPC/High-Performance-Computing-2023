
#if defined(_GNU_SOURCE)
#include <sched.h>
#endif

// ─────────────────────────────────────────────────────────────────
// define the datatype
//
#if !defined(ITYPE)
#warning "compiling with double type"
#define DTYPE  double         // type of data
#define DATYPE double         // type for accumulator
#else
#warning "compiling with int type"
#define DTYPE  unsigned int               // type of data
#define DATYPE long long unsigned int          // type for accumulator
#endif



typedef unsigned long long int uLint;

//
// ------------------------------------------------------------------


#define CONCAT(x,y) x ## y

// ─────────────────────────────────────────────────────────────────
// define the timing routines
//

#define CPU_TIME    (clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ),	\
		     (double)ts.tv_sec +				\
		     (double)ts.tv_nsec * 1e-9)

//
// ------------------------------------------------------------------


// ─────────────────────────────────────────────────────────────────
// define the vector generator
//

#define DEFINE_VECT( T, N, NAME ) typedef T v##NAME __attribute__((vector_size( sizeof(T) * N))); typedef union { v##NAME v; T s[N]; } u##NAME;



// ─────────────────────────────────────────────────────────────────
// define the vector generator
//

#if defined(__GNUC__) && !defined(__ICC) && !defined(__INTEL_COMPILER)
#define PRAGMA_VECT_LOOP _Pragma("GCC ivdep")
#elif defined(__INTEL_COMPILER) | defined(__ICC)
#define PRAGMA_VECT_LOOP _Pragma("parallel")
#elif defined(__clang__)
#define PRAGMA_VECT_LOOP _Pragma("ivdep")
#else
#define PRAGMA_VECT_LOOP
#endif

//
// ------------------------------------------------------------------

// ─────────────────────────────────────────────────────────────────
// 
//



// ─────────────────────────────────────────────────────────────────
// define the debug printing routine
//

#ifdef DEBUG
#define PRINTF(...) printf(__VA_ARGS__)
#define DEBUG_IO 2
#else
#define PRINTF(...)
#endif



DEFINE_VECT( DTYPE, 4, 4d );
DEFINE_VECT( long int, 4, 4i );
