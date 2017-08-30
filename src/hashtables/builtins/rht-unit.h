#pragma once

/* System headers */
#include <stdio.h>
#include <stdbool.h>

/* Project headers */
#include "rht-test.h"


/* Function definition for Unit Tests */
typedef unsigned rhtunit_f (unsigned argc);


/* The structure to keep Unit Tests */
typedef struct
{
  char * name;            /* unique name               */
  char * description;     /* description               */

  rhtunit_f * func;       /* function to run Unit Test */

} rhtunit_t;


/* Returns # of defined Unit Tests */
unsigned rhtunit_no (void);

/* Returns a handle to an Unit Test */
rhtunit_t * rhtunit_first (void);
rhtunit_t * rhtunit_last (void);
rhtunit_t * rhtunit_next (void);
rhtunit_t * rhtunit_next_rnd (void);

rhtunit_t * rhtunit_find_at (unsigned i);
rhtunit_t * rhtunit_find_by_id (unsigned id);
rhtunit_t * rhtunit_find_by_name (char * name);

char ** rhtunit_names (void);
rhtunit_t ** rhtunit_all (void);
rhtunit_t ** rhtunit_all_n (unsigned n);
rhtunit_t ** rhtunit_all_rnd (void);

rhtunit_t * rhtunit_valid (char * id);
void rhtunit_run (rhtunit_t * argv [], unsigned items);
unsigned rhtunit_maxn (rhtunit_t * argv []);
unsigned rhtunit_all_maxn (void);

void rhtunit_print_all (void);
void rhtunit_print (rhtunit_t * argv []);

