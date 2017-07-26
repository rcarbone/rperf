
#pragma once

/* System headers */
#include <stdint.h>
#include <time.h>

/* Project headers */
#include "rtypes.h"


#ifdef __cplusplus
extern "C" {
#endif


/* sprintf() replacement */
void itoa (int c, char * ret);
unsigned digits (unsigned n);


#ifdef __cplusplus
}
#endif
