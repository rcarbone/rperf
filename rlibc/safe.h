
#pragma once

/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#ifdef __cplusplus
extern "C" {
#endif


void * safefree (void * a);
void * safedup (void * a);
void * safereplace (void * a, void * b);
FILE * safeopen (char * f);
FILE * safeclose (FILE * fd);


#ifdef __cplusplus
}
#endif
