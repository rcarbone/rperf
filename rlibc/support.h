
#pragma once

/* System headers */
#include <stdio.h>
#include <stdint.h>
#include <time.h>

/* Project headers */
#include "rtypes.h"


/* Useful macros */
#define RMIN(a, b) (((a) < (b)) ? (a) : (b))
#define RMAX(a, b) (((a) > (b)) ? (a) : (b))


#ifdef __cplusplus
extern "C" {
#endif


/* Evaluate n integers in random order */
unsigned * rndorder (unsigned n);

/* sprintf() replacement */
void itoa (int c, char * ret);
char * utoa (unsigned n);

/* Print left aligned 'name' and leading dots */
void print_dots (char * name, char * label, unsigned n, unsigned seq, unsigned maxn);

/* Evaluate the number of digits in n */
unsigned digits (unsigned n);


#ifdef __cplusplus
}
#endif
