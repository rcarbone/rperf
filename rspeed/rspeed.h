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
#include "varrays.h"
#include "support.h"
#include "datasets.h"
#include "plugins.h"
#include "rtest.h"


/* Definition of a software implementation with all defined testing functions */
typedef struct
{
  char * pathname;       /* shared object with the implementation           */
  rplugin_t * plugin;    /* where all the functions are implemented         */
  char * name;           /* name of implementation as defined in the plugin */
  rtest_t ** suite;      /* suite of tests implemented in the shared object */

} sw_t;


#ifdef __cplusplus
extern "C" {
#endif


/* Public functions in file support.c */
unsigned * rndorder (unsigned n);


/* Public functions in file sw.c */
unsigned sw_maxname (sw_t * sw []);
unsigned sw_no (sw_t * sw []);
unsigned sw_have (sw_t * sw [], char * name);
rtime_t sw_call (sw_t * sw, char * name, unsigned items, robj_t * objs [], bool verbose);
rplugin_f * sw_func (sw_t * sw, char * name);
sw_t ** sw_init (char * argv [], unsigned itesm, bool verbose);
void sw_done (sw_t * implementations [], bool verbose);


/* Public functions in file run.c */
sw_t ** run_suite (char * suite [], sw_t * allsw [], unsigned n, unsigned ntry, bool verbose, bool quiet);


/* Public functions in file print.c */
void print_test_header (unsigned maxn);
void print_test_info (char * label, char * name, unsigned items, unsigned loops);
void print_results (rspent_t * results [], char * name, unsigned t, unsigned maxn, unsigned items, unsigned loops);
void hall_of_fame (char * names [], sw_t * plugins [], unsigned maxn, unsigned items, unsigned loops);


#ifdef __cplusplus
}
#endif
