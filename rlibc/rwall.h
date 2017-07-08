
#pragma once

/* System headers */
#include <stdint.h>
#include <time.h>

/* Project headers */
#include "rtypes.h"


#ifdef __cplusplus
extern "C" {
#endif


/* The current wall time in nsec/usec/msec */
rtime_t nswall (void);
rtime_t uswall (void);
rtime_t mswall (void);

unsigned rrand (unsigned n);
void ** varnd (unsigned argc, void * argv []);


#ifdef __cplusplus
}
#endif
