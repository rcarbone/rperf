
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

/* sprintf() replacement */
void itoa (int c, char * ret);
unsigned digits (unsigned n);


#ifdef __cplusplus
}
#endif
