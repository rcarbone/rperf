#pragma once

/* System headers */
#include <stdio.h>
#include <stdbool.h>

/* Project headers */
#include "rtypes.h"
#include "elems.h"
#include "plugins.h"


/* Useful macros */
#define RMIN(a, b) (((a) < (b)) ? (a) : (b))
#define RMAX(a, b) (((a) > (b)) ? (a) : (b))

#define INITIALS  1e5   /* initial # of elements */
#define LOOPS     10    /* default # of loops per test */


/* Function definition for Unit Tests */
typedef unsigned rlunit_f (unsigned argc);

/* An implementation */
struct sw;

/* The structure to hold execution times for a single test run */
typedef struct
{
  rtime_t t1;           /* The wall time the test was initiated at nsec resolution  */
  rtime_t t2;           /* The wall time the test was completed at nsec resolution  */
  rtime_t elapsed;      /* The time elapsed to complete the test at nsec resolution */
  rtime_t min;          /* Shortest time elapsed */
  rtime_t avg;          /* Average time elapsed  */
  rtime_t max;          /* Longest time elapsed  */
  unsigned rate;

  unsigned elems;       /* The number of elements used to execute the test */
  unsigned slow;        /* A counter used to evaluate a stop condition     */

  struct sw * sw;       /* The implementation under test                   */

} relapsed_t;


/* The structure to keep Unit Tests */
typedef struct
{
  unsigned id;            /* unique id                  */
  char * name;            /* unique name                */
  char * description;     /* description                */

  rlunit_f * unit;         /* function to run Unit Test  */

  relapsed_t ** results;  /* results of test execution  */

} rltest_t;


/* Definition of a software implementation with all defined testing functions */
typedef struct sw
{
  char * pathname;        /* shared object with the implementation               */
  rplugin_t * plugin;     /* where all the functions are implemented             */
  char * name;            /* name of implementation as defined in the plugin     */
  rltest_t ** suite;       /* suite implemented in the shared object and executed */
  unsigned mark;          /* indicator of performances */

} sw_t;


/* Returns # of defined Unit Tests */
unsigned rlunit_no (void);

/* Returns a handle to an Unit Test */
rltest_t * rlunit_first (void);
rltest_t * rlunit_last (void);
rltest_t * rlunit_next (void);
rltest_t * rlunit_next_rnd (void);

rltest_t * rlunit_find_at (unsigned i);
rltest_t * rlunit_find_by_id (unsigned id);
rltest_t * rlunit_find_by_name (char * name);

char ** rlunit_names (void);
rltest_t ** rlunit_all (void);
rltest_t ** rlunit_all_n (unsigned n);
rltest_t ** rlunit_all_rnd (void);

unsigned rlunit_all_maxn (void);
unsigned rlunit_maxn (rltest_t * argv []);

rltest_t * rlunit_valid (char * id);
void rlunit_run (rltest_t * argv [], unsigned elems);


/* Public functions in file elapsed.c */
char * ns2a (uint64_t nsecs);
relapsed_t * mkelapsed (void * sw);
void rmelapsed (void * elapsed);
relapsed_t * dupelapsed (relapsed_t * src);
int sort_by_more_avg (const void * a, const void * b);
int sort_by_less_avg (const void * a, const void * b);
void print_test_header (unsigned maxn);
void show_elapsed (relapsed_t * elapsed);


/* Public functions in file sw.c */
unsigned sw_maxname (sw_t * sw []);
unsigned sw_no (sw_t * sw []);
unsigned sw_have (sw_t * sw [], char * name);
rtime_t sw_call (sw_t * sw, char * name, unsigned n, relem_t * elems [], bool verbose);
rplugin_f * sw_func (sw_t * sw, char * name);
sw_t ** sw_init (char * argv [], unsigned itesm, bool verbose);
void sw_done (sw_t * sw [], bool verbose);


/* Public functions in file print.c */
void rlunit_print_no (void);
void rlunit_print (rltest_t * argv []);
void rlunit_print_all (void);

void print_dots (char * name, char * label, unsigned n, unsigned seq, unsigned maxn);
