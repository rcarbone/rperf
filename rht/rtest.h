#pragma once

/* System headers */
#include <stdio.h>
#include <stdbool.h>

/* Project headers */
#include "rtypes.h"
#include "datasets.h"


/* Useful macros */
#define RMIN(a,b) (a < b ? a : b)
#define RMAX(a,b) (a > b ? a : b)

#if defined(i386)
#define INITIALS  1e4   /* initial # of items */
#else
#define INITIALS  1e5   /* initial # of items */
#endif /* i386 */
#define RUNS      5     /* default # of runs per test */


/* Function definition for Unit Tests and Test Suite */
typedef unsigned runit_f (unsigned argc);
typedef rtime_t rsuite_f (unsigned argc, robj_t * argv []);



/* The structure to hold execution times for a single test run */
typedef struct
{
  rtime_t t1;           /* The wall time the test was initiated at nsec resolution */
  rtime_t t2;           /* The wall time the test was completed at nsec resolution */
  rtime_t elapsed;      /* The time spent to complete the test at nsec resolution  */
  rtime_t min;          /* Shortest time spent */
  rtime_t max;          /* Longest time spent  */
  rtime_t avg;          /* Average time spent  */
  unsigned rate;

  unsigned items;
  unsigned slow;

  void * sw;            /* The implementation under test */

} rspent_t;


/* The structure to hold both Unit Tests and Test Suite */
typedef struct
{
  unsigned id;           /* unique id                  */
  char * name;           /* unique name                */
  char * description;    /* description                */
  runit_f * unit;        /* function to run Unit Test  */
  rsuite_f * suite;      /* function to run Test Suite */
  rspent_t ** results;   /* results of Test execution  */

} rtest_t;


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

rtest_t ** runit_all (void);
rtest_t ** runit_all_n (unsigned n);
rtest_t ** runit_all_rnd (void);

rtest_t * runit_valid (char * id);
void runit_run (rtest_t * argv [], unsigned items);

void runit_print_no (void);
void runit_print (rtest_t * argv []);
void runit_print_all (void);


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

char ** rsuite_names (void);
rtest_t ** rsuite_all (void);
rtest_t ** rsuite_all_n (unsigned n);
rtest_t ** rsuite_all_rnd (void);

rtest_t * rsuite_valid (char * id);
void rsuite_run (rtest_t * suite [], unsigned argc, robj_t * argv []);

void rsuite_print_no (void);
void rsuite_print (rtest_t * argv []);
void rsuite_print_all (void);


/* Public functions in file spent.c */
char * ns2a (uint64_t nsecs);
rspent_t * mkspent (void);
void rmspent (void * spent);
int sort_by_more_avg (const void * a, const void * b);
int sort_by_less_avg (const void * a, const void * b);
void show_spent (rspent_t * spent);
