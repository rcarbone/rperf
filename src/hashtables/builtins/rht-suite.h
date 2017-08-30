#pragma once

/* Project headers */
#include "rtypes.h"
#include "datasets.h"
#include "rht-test.h"


/* Function definition for Test Suite */
typedef rtime_t rhtsuite_f (unsigned argc, void * argv []);


/* The structure to keep Test Suite */
typedef struct
{
  char * name;            /* unique name                */
  char * description;     /* description                */
  rhtsuite_f * func;      /* function to run Test Suite */

  relapsed_t ** results;  /* results of test execution  */

} rhtsuite_t;


/* Returns # of builtins Test Suite */
unsigned rhtsuite_no (void);

/* Returns a handle to a Test Suite */
rhtsuite_t * rhtsuite_first (void);
rhtsuite_t * rhtsuite_last (void);
rhtsuite_t * rhtsuite_next (void);
rhtsuite_t * rhtsuite_next_rnd (void);

rhtsuite_t * rhtsuite_find_at (unsigned i);
rhtsuite_t * rhtsuite_find_by_id (unsigned id);
rhtsuite_t * rhtsuite_find_by_name (char * name);

char ** rhtsuite_all_names (void);
rhtsuite_t ** rhtsuite_all (void);
unsigned rhtsuite_all_maxn (void);
rhtsuite_t ** rhtsuite_all_n (unsigned n);
rhtsuite_t ** rhtsuite_all_rnd (void);
char ** rhtsuite_names (rhtsuite_t * suite []);

rhtsuite_t * rhtsuite_valid (char * id);
unsigned rhtsuite_maxn (rhtsuite_t * argv []);
unsigned rhtsuite_maxd (rhtsuite_t * argv []);

void rhtsuite_run (rhtsuite_t * suite [], unsigned argc, void * argv []);
void rhtsuite_clear_results (rhtsuite_t * suite []);
void rhtsuite_sort_results (rhtsuite_t * suite []);

void rhtsuite_print_all (void);


/* The implementations elsewhere defined */
rhtsuite_f rhtsuite_grow_seq;
rhtsuite_f rhtsuite_grow_rnd;
rhtsuite_f rhtsuite_hit_seq;
rhtsuite_f rhtsuite_hit_rnd;
rhtsuite_f rhtsuite_miss_seq;
rhtsuite_f rhtsuite_miss_rnd;
rhtsuite_f rhtsuite_delete_seq;
rhtsuite_f rhtsuite_delete_rnd;
rhtsuite_f rhtsuite_replace_seq;
rhtsuite_f rhtsuite_replace_rnd;
rhtsuite_f rhtsuite_kbench;
