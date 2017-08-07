#pragma once

/* System headers */
#include <stdio.h>
#include <stdbool.h>

/* Project headers */
#include "rtypes.h"
#include "datasets.h"
#include "plugins.h"


/* Useful macros */
#define RMIN(a, b) (((a) < (b)) ? (a) : (b))
#define RMAX(a, b) (((a) > (b)) ? (a) : (b))

#if defined(i386)
#define INITIALS  1e4   /* initial # of items */
#else
#define INITIALS  1e5   /* initial # of items */
#endif /* i386 */
#define LOOPS     10    /* default # of loops per test */


/* Function definition for Unit Tests and Test Suite */
typedef unsigned runit_f (unsigned argc);
typedef rtime_t rsuite_f (unsigned argc, robj_t * argv []);

/* an implementation */
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

  unsigned items;       /* The number of items used to execute the test */
  unsigned slow;        /* A counter used to evaluate a stop condition  */

  struct sw * sw;       /* The implementation under test                */

} relapsed_t;


/* The structure to hold both Unit Tests and Test Suite */
typedef struct
{
  unsigned id;            /* unique id                  */
  char * name;            /* unique name                */
  char * description;     /* description                */

  runit_f * unit;         /* function to run Unit Test  */
  rsuite_f * suite;       /* function to run Test Suite */

  relapsed_t ** results;  /* results of test execution  */

} rtest_t;


/* Definition of a software implementation with all defined testing functions */
typedef struct sw
{
  char * pathname;        /* shared object with the implementation               */
  rplugin_t * plugin;     /* where all the functions are implemented             */
  char * name;            /* name of implementation as defined in the plugin     */
  rtest_t ** suite;       /* suite implemented in the shared object and executed */
  unsigned mark;          /* indicator of performances */

} sw_t;


/* Returns # of defined Unit Tests */
unsigned runit_no (void);

/* Returns a handle to an Unit Test */
rtest_t * runit_first (void);
rtest_t * runit_last (void);
rtest_t * runit_next (void);
rtest_t * runit_next_rnd (void);

rtest_t * runit_find_at (unsigned i);
rtest_t * runit_find_by_id (unsigned id);
rtest_t * runit_find_by_name (char * name);

char ** runit_names (void);
rtest_t ** runit_all (void);
rtest_t ** runit_all_n (unsigned n);
rtest_t ** runit_all_rnd (void);

rtest_t * runit_valid (char * id);
void runit_run (rtest_t * argv [], unsigned items);


/* Returns # of defined Test Suite */
unsigned rsuite_no (void);

/* Returns a handle to a Test Suite */
rtest_t * rsuite_first (void);
rtest_t * rsuite_last (void);
rtest_t * rsuite_next (void);
rtest_t * rsuite_next_rnd (void);

rtest_t * rsuite_find_at (unsigned i);
rtest_t * rsuite_find_by_id (unsigned id);
rtest_t * rsuite_find_by_name (char * name);

char ** rsuite_all_names (void);
rtest_t ** rsuite_all (void);
rtest_t ** rsuite_all_n (unsigned n);
rtest_t ** rsuite_all_rnd (void);
char ** rsuite_names (rtest_t * suite []);

rtest_t * rsuite_valid (char * id);
unsigned rsuite_maxn (rtest_t * argv []);
unsigned rsuite_maxd (rtest_t * argv []);

void rsuite_run (rtest_t * suite [], unsigned argc, robj_t * argv []);
void rsuite_clear_results (rtest_t * suite []);
void rsuite_sort_results (rtest_t * suite []);


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
rtime_t sw_call (sw_t * sw, char * name, unsigned items, robj_t * objs [], bool verbose);
rplugin_f * sw_func (sw_t * sw, char * name);
sw_t ** sw_init (char * argv [], unsigned itesm, bool verbose);
void sw_done (sw_t * implementations [], bool verbose);


/* Public functions in file print.c */
void runit_print_no (void);
void runit_print (rtest_t * argv []);
void runit_print_all (void);

void rsuite_print_no (void);
void rsuite_print (rtest_t * argv []);
void rsuite_print_all (void);

void print_dots (char * name, char * label, unsigned n, unsigned seq, unsigned maxn);
