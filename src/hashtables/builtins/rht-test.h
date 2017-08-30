#pragma once

/* System headers */
#include <stdio.h>
#include <stdbool.h>

/* Project headers */
#include "support.h"
#include "rtypes.h"
#include "datasets.h"


/* Useful macros */
#define INITIALS  1e4   /* initial # of items */
#define LOOPS     10    /* default # of loops per test */

#define SEP  '|'


/* Opaque structure used to represent an implementation */
struct sw;


/* The structure to keep execution times for a single test run */
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


/* Public functions in file elapsed.c */
char * ns2a (uint64_t nsecs);
relapsed_t * mkelapsed (void * sw);
void rmelapsed (void * elapsed);
relapsed_t * dupelapsed (relapsed_t * src);
int sort_by_more_avg (const void * a, const void * b);
int sort_by_less_avg (const void * a, const void * b);
void show_elapsed (relapsed_t * elapsed);

void print_test_header (unsigned maxn);
