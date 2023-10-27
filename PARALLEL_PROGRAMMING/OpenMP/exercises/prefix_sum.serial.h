
// ─────────────────────────────────────────────────────────────────
// define the datatype
//
#if !defined(DTYPE)
#define DTYPE double
#endif

typedef unsigned int uint;


// ─────────────────────────────────────────────────────────────────
// define the timing routines
//

#define CPU_TIME ({struct timespec ts;					\
    clock_gettime( CLOCK_PROCESS_CPUTIME_ID, &ts ),			\
      (double)ts.tv_sec +						\
      (double)ts.tv_nsec * 1e-9;})


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


// ─────────────────────────────────────────────────────────────────
// prototypes
//

double scan           ( const uint, DTYPE * restrict );
double scan_efficient ( const uint, DTYPE * restrict );

