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


/* Public functions in file run.c */
sw_t ** run_suite (char * suite [], sw_t * sw [],
		   unsigned loops, unsigned initials,
		   unsigned nslow, unsigned repeat, unsigned more,
		   bool verbose, bool quiet);


/* Public functions in file print.c */
void print_results (rspent_t * results [], char * name, unsigned maxn, unsigned items, unsigned loops);
void hall_of_fame (char * name, rspent_t * results [], unsigned all, unsigned maxn,
		   unsigned initials, unsigned repeat, unsigned more);



#ifdef __cplusplus
}
#endif
