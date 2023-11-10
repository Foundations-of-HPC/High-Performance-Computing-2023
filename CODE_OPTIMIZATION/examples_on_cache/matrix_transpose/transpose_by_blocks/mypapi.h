

#if defined(USE_PAPI)                                           // -----------------------------------------------------------
#include <papi.h>

typedef unsigned long long int uLint;

#define PAPI_EVENTS_NUM 4
int   papi_events[PAPI_EVENTS_NUM] = {PAPI_TOT_INS, PAPI_TOT_CYC, PAPI_L1_DCM, PAPI_L2_DCM };
int   papi_EventSet                = PAPI_NULL;             // the handle for the events' set
uLint papi_buffer[PAPI_EVENTS_NUM] = {0};                   // storage for the counters' values
uLint papi_values[PAPI_EVENTS_NUM] = {0};                   // accumulate the counters' values

                                                                // check that PAPI is OK, exit if not
#define PAPI_CHECK( R ) {						\
    if ( (R) != PAPI_OK ) {						\
      printf("a problem with PAPI (code %d) arise at line %d\n",	\
	     (R), __LINE__);fflush(stdout); return (R); }}


                                                                // check that PAPI is OK,
                                                                // issue a warning if not with a
                                                                // provided message
#define PAPI_WARN( R, S ) {						\
    if ( (R) != PAPI_OK ) {						\
      printf("a problem  with PAPI (code %d) arise at line %d: %s\n",	\
	     (R),  __LINE__, (S)); fflush(stdout); }}

                                                                // check that PAPI is OK about an event
                                                                // issue a warning if not with a
                                                                // provided message
#define PAPI_WARN_EVENT( R, E, S1, n ) {				\
    if ( (R) != PAPI_OK ) {						\
      printf("a problem  with PAPI (code %d) : event %d arise at line %d: %s (%d)\n", \
	     (R), (E),  __LINE__, (S1), (n)); fflush(stdout); }}


#define PAPI_ADD_EVENTS_to_SET { for ( int i = 0; i < PAPI_EVENTS_NUM; i++) { \
      retval = PAPI_query_event(papi_events[i]);			\
      if ( retval == PAPI_OK ) {					\
	retval = PAPI_add_event(papi_EventSet, papi_events[i]);		\
	PAPI_WARN_EVENT(retval, papi_events[i], "adding event", i);} else { \
      PAPI_WARN_EVENT(retval, papi_events[i],"querying event", i)}  } }

#define PAPI_INIT {							\
    int retval = PAPI_library_init(PAPI_VER_CURRENT);			\
    if (retval != PAPI_VER_CURRENT)					\
      printf("wrong PAPI initialization: version %d instead of %d has been found\n", retval, PAPI_VER_CURRENT); \
    retval = PAPI_create_eventset(&papi_EventSet); PAPI_WARN(retval,"creating event set"); \
    PAPI_ADD_EVENTS_to_SET; }

// to use HIGH-LEVEL API
//#define PAPI_START_CNTR { int res = PAPI_start_counters(papi_events, PAPI_EVENTS_NUM); PAPI_CHECK_RES(res); }
//#define PAPI_STOP_CNTR  { int res = PAPI_stop_counters(papi_values, PAPI_EVENTS_NUM); PAPI_CHECK_RES(res); }

// to use NORMAL API
#define PAPI_START_CNTR {						\
    int retval = PAPI_start(papi_EventSet); PAPI_WARN(retval, "starting counters"); }

#define PAPI_STOP_CNTR {						\
int retval = PAPI_stop(papi_EventSet, papi_buffer);			\
if( retval == PAPI_OK ) {						\
  for( int jj = 0; jj < PAPI_EVENTS_NUM; jj++)				\
    papi_values[jj] += papi_buffer[jj]; } else PAPI_WARN(retval, "reading counters"); }

#define PAPI_GET_CNTR( i ) ( papi_values[(i)] )

#define PAPI_FLUSH_BUFFER {				\
    for( int jj = 0; jj < PAPI_EVENTS_NUM; jj++)	\
      papi_buffer[ jj] = 0; }

#define PAPI_FLUSH {					\
    for( int jj = 0; jj < PAPI_EVENTS_NUM; jj++)	\
      papi_values[jj] = papi_buffer[ jj] = 0; }


#else                                                           // -----------------------------------------------------------

#define PAPI_EVENTS_NUM 0
#define PAPI_INIT
#define PAPI_START_CNTR
#define PAPI_STOP_CNTR
#define PAPI_FLUSH
#define PAPI_GET_CNTR( i ) 0

#endif                                                          // -----------------------------------------------------------
