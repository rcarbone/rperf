
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

/* Well formatted percentage */
char * xpercentage (unsigned long partial, unsigned long total);

char * rfqname (void);
void rhelloworld (char * progname, char * version, char * date, char * time, char * nodename, char * author);
void rversion (char * progname, char * version, char * date, char * time);

char * scenter (char * s, unsigned max);


#ifdef __cplusplus
}
#endif
