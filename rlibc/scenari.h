
#pragma once

/* System headers */
#include <stdio.h>
#include <stdbool.h>

/* Private headers */
#include "rtypes.h"


/* The structure to hold information for running a test */
typedef struct
{
  /* Static values */
  unsigned id;           /* unique id           */
  char * name;           /* unique name         */
  char * description;    /* description         */
  rmain_f * fun;         /* function to run it  */

  bool enabled;          /* enable/disable flag */

  /* Dynamic values */
  rtime_t t1;           /* The wall time in nsecs the test was initiated */
  rtime_t t2;           /* The wall time in nsecs the test was completed */
  rtime_t elapsed;      /* The time spent in nsecs to complete the test  */
  rtime_t min;          /* Shortest time spent */
  rtime_t max;          /* Longest time spent  */
  rtime_t avg;          /* Average time spent  */

} rtest_t;
