#pragma once

/* System headers */
#include <stdio.h>
#include <stdbool.h>

/* Project headers */
#include "rltest.h"


/* Function definition for Unit Tests */
typedef unsigned rlunit_f (unsigned argc);


/* The structure to keep Unit Tests */
typedef struct
{
  char * name;            /* unique name                */
  char * description;     /* description                */
  rlunit_f * unit;        /* function to run Unit Test  */

  relapsed_t ** results;  /* results of test execution  */

} rlunit_t;


/* Returns # of defined Unit Tests */
unsigned rlunit_no (void);

/* Returns a handle to an Unit Test */
rlunit_t * rlunit_first (void);
rlunit_t * rlunit_last (void);
rlunit_t * rlunit_next (void);
rlunit_t * rlunit_next_rnd (void);

rlunit_t * rlunit_find_at (unsigned i);
rlunit_t * rlunit_find_by_id (unsigned id);
rlunit_t * rlunit_find_by_name (char * name);

char ** rlunit_names (void);
rlunit_t ** rlunit_all (void);
rlunit_t ** rlunit_all_n (unsigned n);
rlunit_t ** rlunit_all_rnd (void);

unsigned rlunit_all_maxn (void);
unsigned rlunit_maxn (rlunit_t * argv []);

rlunit_t * rlunit_valid (char * id);
void rlunit_run (rlunit_t * argv [], unsigned elems);


/* Public functions in file print.c */
void rlunit_print_no (void);
void rlunit_print (rlunit_t * argv []);
void rlunit_print_all (void);
