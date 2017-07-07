#pragma once

/* System headers */
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <getopt.h>

/* rlibc public headers */
#include "varrays.h"
#include "support.h"

#include "datasets.h"
#include "plugins.h"


/* Useful macros */
#define MIN(a,b) (a < b ? a : b)
#define MAX(a,b) (a > b ? a : b)


/* The type definition for a test function */
typedef uint64_t run_f (robj_t * objs, unsigned n);


/* The structure to hold information for running a test */
typedef struct
{
  /* Static values */
  unsigned id;           /* unique test id      */
  char * name;           /* unique test name    */
  char * description;    /* test description    */
  call_t * fun;          /* function to run it  */

  bool enabled;          /* enable/disable flag */

  /* Dynamic values */
  rtime_t t1;           /* The wall time the test was initiateed at nsec resolution */
  rtime_t t2;           /* The wall time the test was completed at nsec resolution  */
  rtime_t elapsed;      /* The time spent to complete the test at nsec resolution   */
  rtime_t min;          /* Shortest time spent */
  rtime_t max;          /* Longest time spent  */
  rtime_t avg;          /* Average time spent  */
  unsigned rate;

  unsigned items;
  unsigned slow;

} test_t;


/* Definition of a software implementation with all defined testing functions */
typedef struct
{
  bool enabled;          /* enable/disable flag     */

  char * pathname;       /* shared object with the implementation   */
  plugin_t * plugin;     /* where all the functions are implemented */
  char * name;           /* name of implementation  */
  test_t ** suite;       /* suite of tests implemented in the shared object */

} sw_t;


/* Definition in order keep the results of the test execution and to evaluate the best implementation for a given test */
typedef struct
{
  sw_t * sw;             /* implementation under test */
  test_t * test;         /* last test executed        */

} result_t;



#ifdef __cplusplus
extern "C" {
#endif


/* Public functions in file options.c */
unsigned optmax (struct option * options);
char * optlegitimate (struct option * options);
char * optname (struct option * options, unsigned n, unsigned val);
void usage_item (struct option * options, unsigned n, unsigned val, char * description);


/* Public functions in file support.c */
void init_keys (unsigned n, bool sparse);
void done_keys (void);

unsigned * rndorder (unsigned n);


/* Public functions in file suite.c */
unsigned tno (void);
char * tname (unsigned id);
char * tdescription (unsigned id);


/* Public functions in file sw.c */
unsigned sw_maxname (sw_t * sw []);
unsigned sw_no (sw_t * sw []);
bool sw_call (sw_t * sw, char * name, unsigned items, robj_t * objs [], bool verbose);
call_t * sw_func (sw_t * sw, char * name);
sw_t ** sw_init (char * argv [], unsigned itesm, bool verbose);
void sw_done (sw_t * implementations [], bool verbose);
test_t * get_test (sw_t * sw, unsigned id);
unsigned implemented (sw_t * sw [], unsigned id);


/* Public functions in file elapsed.c */
char * ns2a (uint64_t nsecs);


/* Public functions in file run.c */
sw_t ** run_all_tests (sw_t * allsw [], unsigned n,
		       unsigned ntry, unsigned nslow, unsigned more1, unsigned more2,
		       bool verbose, bool less, bool show);


/* Public functions in file results.c */
result_t ** mkmatrix (unsigned rows, unsigned cols);
void rmmatrix (result_t ** matrix, unsigned rows);
int sort_by_avg (const void * a, const void * b);
int sort_by_rev_avg (const void * a, const void * b);
int sort_by_items (const void * a, const void * b);
int sort_by_rev_items (const void * a, const void * b);
int sort_by_spent (const void * a, const void * b);
int sort_by_less_spent (const void * a, const void * b);


/* Public functions in file print.c */
void old_show_info (sw_t * sw, test_t * test, unsigned maxn);
void hall_of_fame (unsigned t, result_t ** results, unsigned all, unsigned maxn,
		   unsigned initials, unsigned repeat, unsigned more);



#ifdef __cplusplus
}
#endif
