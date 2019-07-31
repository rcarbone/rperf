
#pragma once


/* System headers */
#include <stdbool.h>
#include <string.h>


/* Project headers */
#include "vargv.h"
#include "sargv.h"

#if !defined(RMAX)
#define RMAX(a, b)  (((a) > (b)) ? (a) : (b))
#endif


#ifdef __cplusplus
extern "C" {
#endif


unsigned argslen (char * argv []);
char ** argsmore (char * argv [], char * s);
char ** argsless (char * argv [], char * s);
int argshas (char * argv [], char * item);
char ** argsclear (char * argv []);
char ** argsdup (char * argv []);
char ** argscat (char * a [], char * b []);
char ** argssort (char * argv []);
char * argsjoin (char * argv []);
char ** argssplit (char * str, char * sep);
char ** argspieces (char * list, char * sep);
char ** argsblanks (char * list);
char ** argsuniq (char * argv [], char * item);
bool argsexists (char * argv [], char * name);

void argsrows (char * argv []);
unsigned argslongest (char * argv []);
void argscols (char * argv []);
void args_2d_rows (char * argv []);

#ifdef __cplusplus
}
#endif
