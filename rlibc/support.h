
#pragma once

/* System headers */
#include <stdio.h>
#include <stdint.h>
#include <time.h>

/* Project headers */
#include "rtypes.h"


#ifdef __cplusplus
extern "C" {
#endif


/* Evaluate n integers in random order */
unsigned * rndorder (unsigned n);

/* sprintf() replacement */
void itoa (int c, char * ret);
char * utoa (unsigned n);

/* Evaluate the number of digits in n */
unsigned digits (unsigned n);


#ifdef __cplusplus
}
#endif
