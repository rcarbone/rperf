/* System headers */
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

/* Project headers */
#include "rtest.h"
#include "safe.h"


/* ============================= */

/* Evaluate decimal value of n according to n (eg. t=123456 n=1000 => 4.5) */
static uint64_t ntrunc (uint64_t t, unsigned n)
{
  return (t - (t / n) * n) / (n / 10);            /* just 1 digit */
}


/* Convert nanoseconds in microseconds */
static uint64_t n2us (uint64_t t)
{
  return t / 1000;               /* 1e3 */
}


/* Convert nanoseconds in millseconds */
uint64_t n2ms (uint64_t t)
{
  return t / 1000000;            /* 1e6 */
}


/* Convert nanoseconds in seconds */
static uint64_t n2ss (uint64_t t)
{
  return t / 1000000000;         /* 1e9 */
}


/* Convert nanoseconds in minutes */
static uint64_t n2mm (uint64_t t)
{
  return t / 1000000000 / 60;
}


/* Convert nanoseconds in hours */
static uint64_t n2hh (uint64_t t)
{
  return t / 1000000000 / 60 / 60;
}


/* Convert nanoseconds in days */
static uint64_t n2dd (uint64_t t)
{
  return t / 1000000000 / 60 / 60 / 24;
}


/* Eval seconds in minutes */
static unsigned ssinn (uint64_t t)
{
  return n2ss (t - (n2mm (t) * 60 * 1000000000));
}


/* Eval minutes in hours
 *
 * Devo calcolare:
 *  quante ore ci sono in t               => hh = n2hh (t)
 *  quanti ns ci sono nelle ore calcolate => ns = n2hh (t) * 60 * 60 * 1000000000
 *  la differenza in ns tra di 2 valori   => dt = t - n2hh (t) * 60 * 60 * 1000000000
 *  quanti minuti ci sono nel dt          => mm = n2mm (t - n2hh (t) * 60 * 60 * 1000000000)
 *
 */
static unsigned mminn (uint64_t t)
{
  return n2mm (t - (n2hh (t) * 60 * 60 * 1000000000));
}


static unsigned hhinn (uint64_t t)
{
  return n2hh (t - (n2dd (t) * 24 * 60 * 60 * 1000000000));
}


/* Return a well formatted string rapresenting given time at nanosecond resolution */
char * ns2a (uint64_t nsecs)
{
  static char elapsed [64];

  if (nsecs < 1e3)
    sprintf (elapsed, "%3ju ns", nsecs);
  else if (nsecs < 1e6)
    sprintf (elapsed, "%3ju.%1ju us",  n2us (nsecs), ntrunc (nsecs, 1000));
  else if (nsecs < 1e9)
    sprintf (elapsed, "%3ju.%1ju ms",  n2ms (nsecs), ntrunc (nsecs, 1000000));
  else if (nsecs < 60 * 1e9)
    sprintf (elapsed, "%3ju.%1ju s ",  n2ss (nsecs), ntrunc (nsecs, 1000000000));
  else if (nsecs < 60 * 60 * 1e9)
    sprintf (elapsed, "%3ju:%02u mm",  n2mm (nsecs), ssinn (nsecs));
  else if (nsecs < 24 * 60 * 60 * 1e9)
    sprintf (elapsed, "%3ju:%02u hh",  n2hh (nsecs), mminn (nsecs));
  else
    sprintf (elapsed, "%3ju, %02u dd", n2dd (nsecs), hhinn (nsecs));

  return elapsed;
}


/* Memory allocator/deallocator for the rspent_t variables. */
rspent_t * mkspent (void)
{
  rspent_t * spent = calloc (1, sizeof (* spent));
  spent -> min = ULONG_MAX;
  return spent;
}


void rmspent (void * spent)
{
  safefree (spent);
}


/* Sort test results by avg time spent */
int sort_by_more_avg (const void * a, const void * b)
{
  return (* (rspent_t **) b) -> avg - (* (rspent_t **) a) -> avg;
}


int sort_by_less_avg (const void * a, const void * b)
{
  return sort_by_more_avg (b, a);
}


/* Display test header */
void print_test_header (unsigned maxn)
{
  printf ("      %-*.*s -   avg         min         max   ms -   elapsed\n",
	  maxn, maxn, "Implementation");
}


/* Display timing information for a single test run */
void show_spent (rspent_t * spent)
{
  if (spent)
    printf (" %8.3f  / %8.3f  / %8.3f    - %12.12s\n",
	    spent -> avg / 1e6,
	    spent -> min / 1e6,
	    spent -> max / 1e6,
	    ns2a (spent -> elapsed));
}


#if defined(TEST)
int main (int argc, char * argv [])
{
  uint64_t t;

  /* ===================== */

  /* 1 nanosecondo (lower limit) */
  t = 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 999 nanosecondi (upper limit) */
  t = 1e3 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 microsecondo (lower limit) */
  t = 1e3;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 50 * 1e3 + 123456;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 999 microsecondi (upper limit) */
  t = 1e6 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 millisecondo (lower limit) */
  t = 1e6;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 50 * 1e6 + 123456789;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 999 millisecondi (upper limit) */
  t = 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 secondo (lower limit) */
  t = 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 30 * 1e9 + 987654321;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 59 secondi (upper limit) */
  t = 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 minuto (lower limit) */
  t = 60 * 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 30 * 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 59 minuti (upper limit) */
  t = 60 * 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 ora (lower limit) */
  t = 60 * 60 * 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* about half */
  t = 12 * 60 * 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 23 ore (lower limit) */
  t = 24 * 60 * 60 * 1e9 - 1;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  printf ("\n");

  /* ===================== */

  /* 1 giorno (lower limit) */
  t = 1 * 24 * 60 * 60 * 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  /* 30 giorni (lower limit) */
  t = 30 * 24 * 60 * 60 * 1e9;
  printf ("%16ju -> [%s]\n", t, ns2a (t));

  return 0;
}

/* gcc -DTEST elapsed.c -o elapsed && ./elapsed */

#endif /* TEST */
