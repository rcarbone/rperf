#pragma once

/* Project headers */
#include "rtypes.h"
#include "elems.h"
#include "rltest.h"


/* Function definition for Test Suite */
typedef rtime_t rlsuite_f (unsigned argc, void * argv []);


/* The structure to keep Test Suite */
typedef struct
{
  char * name;            /* unique name                */
  char * description;     /* description                */
  rlsuite_f * func;       /* function to run Test Suite */

  relapsed_t ** results;  /* results of test execution  */

} rlsuite_t;


/* Returns # of builtins Test Suite */
unsigned rlsuite_no (void);

/* Returns a handle to a Test Suite */
rlsuite_t * rlsuite_first (void);
rlsuite_t * rlsuite_last (void);
rlsuite_t * rlsuite_next (void);
rlsuite_t * rlsuite_next_rnd (void);

rlsuite_t * rlsuite_find_at (unsigned i);
rlsuite_t * rlsuite_find_by_id (unsigned id);
rlsuite_t * rlsuite_find_by_name (char * name);

char ** rlsuite_all_names (void);
rlsuite_t ** rlsuite_all (void);
unsigned rlsuite_all_maxn (void);
rlsuite_t ** rlsuite_all_n (unsigned n);
rlsuite_t ** rlsuite_all_rnd (void);

rlsuite_t * rlsuite_valid (char * id);

char ** rlsuite_names (rlsuite_t * suite []);
unsigned rlsuite_maxn (rlsuite_t * argv []);
unsigned rlsuite_maxd (rlsuite_t * argv []);

void rlsuite_run (rlsuite_t * suite [], unsigned argc, void * argv []);
void rlsuite_clear_results (rlsuite_t * suite []);
void rlsuite_sort_results (rlsuite_t * suite []);

void rlsuite_print_all (void);


/* The implementations elsewhere defined */
rlsuite_f rlsuite_grow_head;
rlsuite_f rlsuite_grow_tail;
rlsuite_f rlsuite_iterate;
rlsuite_f rlsuite_pop_head;
rlsuite_f rlsuite_pop_tail;
