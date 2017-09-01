#pragma once

/* System headers */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <getopt.h>

/* Project headers */
#include "vargv.h"
#include "support.h"
#include "datasets.h"
#include "plugins.h"
#include "rht-suite.h"


/* Definition of a software implementation with all defined testing functions */
typedef struct sw
{
  char * pathname;        /* shared object with the implementation               */
  rplugin_t * plugin;     /* where all the functions are implemented             */
  char * name;            /* name of implementation as defined in the plugin     */
  rhtsuite_t ** suite;    /* suite implemented in the shared object and executed */
  unsigned mark;          /* indicator of performances */

} sw_t;


#ifdef __cplusplus
extern "C" {
#endif


/* Public functions in file run.c */
sw_t ** run_suite (rhtsuite_t * suite [], sw_t * sw [],
		   unsigned loops, unsigned initials,
		   unsigned nslow, unsigned repeat, unsigned more,
		   bool verbose, bool quiet);


/* Public functions in file sw.c */
unsigned sw_maxname (sw_t * sw []);
unsigned sw_no (sw_t * sw []);
unsigned sw_have (sw_t * sw [], char * name);
rtime_t sw_call (sw_t * sw, char * name, unsigned items, void * objs [], bool verbose);
rplugin_f * sw_func (sw_t * sw, char * name);
sw_t ** sw_init (char * argv [], unsigned itesm, bool verbose);
void sw_done (sw_t * sw [], bool verbose);


/* Public functions in file print.c */
void print_results (relapsed_t * results [], char * name, unsigned maxn, unsigned items, unsigned loops);
void hall_of_fame (char * name, relapsed_t * results [], unsigned all, unsigned maxn,
		   unsigned initials, unsigned repeat, unsigned more);



#ifdef __cplusplus
}
#endif
