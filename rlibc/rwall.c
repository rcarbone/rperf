/* System headers */
#include <stdlib.h>

/* Public header */
#include "rwall.h"


/* Return the current wall time in nanoseconds */
rtime_t nswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts . tv_sec * 1e9 + ts . tv_nsec;
}


/* Return the current wall time in microseconds */
rtime_t uswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts . tv_sec * 1e6 + ts . tv_nsec / 1e3;
}


/* Return the current wall time in milliseconds */
rtime_t mswall (void)
{
  struct timespec ts;
  clock_gettime (CLOCK_MONOTONIC_RAW, & ts);
  return ts . tv_sec * 1e3 + ts . tv_nsec / 1e6;
}


/* Evaluate a random number in the range [0 n-1] */
unsigned rrand (unsigned n)
{
  srand (nswall ());
  return rand () % n;
}


/*
 * http://stackoverflow.com/questions/6127503/shuffle-array-in-c
 *
 * Arrange the N elements of ARRAY in random order.
 */
void ** varnd (unsigned argc, void * argv [])
{
  unsigned i;
  unsigned j;
  void * t;

  srand (nswall ());

  if (argc > 1)
    {
      for (i = 0; i < argc - 1; i ++)
        {
          j = i + rand () / (RAND_MAX / (argc - i) + 1);
          t = argv [j];
          argv [j] = argv [i];
          argv [i] = t;
        }
    }
  return argv;
}
