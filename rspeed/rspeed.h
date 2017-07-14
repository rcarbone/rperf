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


#ifdef __cplusplus
extern "C" {
#endif


/* Public functions in file support.c */
unsigned * rndorder (unsigned n);


/* Public functions in file run.c */
sw_t ** run_suite (char * suite [], sw_t * allsw [], unsigned n, unsigned ntry, bool verbose, bool quiet);


/* Public functions in file print.c */
void print_test_info (char * label, char * name, unsigned items, unsigned loops);
void print_results (rspent_t * results [], char * name, unsigned maxn, unsigned items, unsigned loops);
void hall_of_fame (char * names [], sw_t * plugins [], unsigned maxn, unsigned items, unsigned loops);


#ifdef __cplusplus
}
#endif
