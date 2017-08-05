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
#include "vargs.h"
#include "support.h"
#include "datasets.h"
#include "plugins.h"
#include "rtest.h"


#ifdef __cplusplus
extern "C" {
#endif


/* Public functions in file run.c */
sw_t ** run_suite (rtest_t * suite [], sw_t * sw [], unsigned loops, unsigned items, bool verbose, bool quiet);


/* Public functions in file print.c */
void print_test_info (char * label, char * name, unsigned loops, unsigned items, unsigned maxn);
void print_results (rspent_t * results [], char * name, unsigned maxn, unsigned loops, unsigned items);
void hall_of_fame (rtest_t * suite [], sw_t * plugins [], unsigned maxn, unsigned loops, unsigned items);


#ifdef __cplusplus
}
#endif
