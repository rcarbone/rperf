/*
 * All the Test Suite are here, grouped together in a static table.
 *
 * Each implementation of a single Test Suite operates with 'n' different unique keys
 * (except kbench which requires a dataset of non-unique keys).
 */


/* Project headers */
#define NEED_RHT_TYPEDEF
#include "rht-api.h"
#include "sargv.h"
#include "rwall.h"
#include "rctype.h"
#include "support.h"
#include "datasets.h"
#include "rht-suite.h"


/* All the Test Suite in a static table */
static rhtsuite_t builtins [] =
{
  { "grow_seq",    "Populate an empty container (sequential keys)",           rhtsuite_grow_seq    },
  { "grow_rnd",    "Populate an empty container (random keys)",               rhtsuite_grow_rnd    },
  { "hit_seq",     "Search one-by-one all existing items (sequential keys)",  rhtsuite_hit_seq     },
  { "hit_rnd",     "Search one-by-one all existing items (random keys)",      rhtsuite_hit_rnd     },
  { "miss_seq",    "Search for non existing items (sequential keys)",         rhtsuite_miss_seq    },
  { "miss_rnd",    "Search for non existing items (random keys)",             rhtsuite_miss_rnd    },
  { "delete_seq",  "Delete one-by-one all existing items (sequential keys)",  rhtsuite_delete_seq  },
  { "delete_rnd",  "Delete one-by-one all existing items (random keys)",      rhtsuite_delete_rnd  },
  { "replace_seq", "Replace one-by-one all existing items (sequential keys)", rhtsuite_replace_seq },
  { "replace_rnd", "Replace one-by-one all existing items (random keys)",     rhtsuite_replace_rnd },
  { "kbench",      "Delete if found, add otherwise (non-unique keys)",        rhtsuite_kbench      },
};
#define RHTSUITE_NO (sizeof (builtins) / sizeof (* builtins))


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


static void rhtsuite_run_one (rhtsuite_t * test, unsigned argc, void * argv [],
			      unsigned n, unsigned seq, unsigned maxn)
{
  rtime_t elapsed;

  print_dots (test -> name, "Running", n, seq, maxn);

  elapsed = test -> func (argc, argv);
  if (elapsed)
    printf ("Ok - %s\n", ns2a (elapsed));
  else
    printf ("No\n");
}


static void rhtsuite_print_header (unsigned maxn)
{
  printf (" # %c %-*.*s %c %s %c %s\n", SEP, maxn, maxn, "Name", SEP, "Id", SEP, "Description");
  printf ("--- %s %c %s %c %s\n", "--------", SEP, "--", SEP, "----------------------");
}


static void rhtsuite_print_one (rhtsuite_t * suite, unsigned n, unsigned maxn)
{
  printf ("%3d%c %-*.*s %c%3d %c %s\n", n, SEP, maxn, maxn, suite -> name, SEP, n, SEP, suite -> description);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* Return the # of Test Suite */
unsigned rhtsuite_no (void)
{
  return RHTSUITE_NO;
}


/* Return the handle of the first Test Suite */
rhtsuite_t * rhtsuite_first (void)
{
  return & builtins [0];
}


/* Return the handle of the last Test Suite */
rhtsuite_t * rhtsuite_last (void)
{
  return & builtins [RHTSUITE_NO - 1];
}


/* Return the handle of the next Test Suite */
rhtsuite_t * rhtsuite_next (void)
{
  static unsigned i = 0;
  return & builtins [i ++ % RHTSUITE_NO];
}


/* Return the handle of the next Test Suite randomly generated */
rhtsuite_t * rhtsuite_next_rnd (void)
{
  return & builtins [rrand (RHTSUITE_NO)];
}


/* Looukup for a Test Suite by index */
rhtsuite_t * rhtsuite_find_at (unsigned i)
{
  return i < RHTSUITE_NO ? & builtins [i] : NULL;
}


/* Looukup for a Test Suite by its unique id */
rhtsuite_t * rhtsuite_find_by_id (unsigned id)
{
  return id > 0 && id <= RHTSUITE_NO ? & builtins [id - 1] : NULL;
}


/* Looukup for a Test Suite by its unique name */
rhtsuite_t * rhtsuite_find_by_name (char * name)
{
  unsigned i;
  for (i = 0; name && i < RHTSUITE_NO; i ++)
    if (! strcmp (builtins [i] . name, name))
      return & builtins [i];
  return NULL;
}


/* Return all the Test Suite names in an array in the same order they were defined */
char ** rhtsuite_all_names (void)
{
  char ** all = NULL;
  unsigned i;
  for (i = 0; i < RHTSUITE_NO; i ++)
    all = argsmore (all, builtins [i] . name);
  return all;
}


/* Return all the Test Suite in an array in the same order they were defined */
rhtsuite_t ** rhtsuite_all (void)
{
  rhtsuite_t ** all = NULL;
  unsigned i;
  for (i = 0; i < RHTSUITE_NO; i ++)
    all = arrmore (all, & builtins [i], rhtsuite_t);
  return all;
}


/* Longest name */
unsigned rhtsuite_all_maxn (void)
{
  unsigned n = 0;
  unsigned i;
  for (i = 0; i < RHTSUITE_NO; i ++)
    n = RMAX (n, strlen (builtins [i] . name));
  return n;
}


/* Return all the Test Suite in an array in the same order they were defined starting at given offset */
rhtsuite_t ** rhtsuite_all_n (unsigned n)
{
  rhtsuite_t ** all;
  unsigned i;

  n %= RHTSUITE_NO;

  for (i = 0; i < n; i ++)
    rhtsuite_next ();

  all = arrmore (NULL, rhtsuite_next (), rhtsuite_t);
  while (arrlen (all) != rhtsuite_no ())
    all = arrmore (all, rhtsuite_next (), rhtsuite_t);
  return all;
}


rhtsuite_t ** rhtsuite_all_rnd (void)
{
  return (rhtsuite_t **) varnd (rhtsuite_no (), (void **) rhtsuite_all ());
}


/* Return the Test Suite names in the same order they were defined in the table */
char ** rhtsuite_names (rhtsuite_t * suite [])
{
  char ** all = NULL;
  while (suite && * suite)
    all = argsmore (all, (* suite ++) -> name);
  return all;
}


rhtsuite_t * rhtsuite_valid (char * id)
{
  return isnumeric (id) && atoi (id) ? rhtsuite_find_at (atoi (id) - 1) : rhtsuite_find_by_name (id);
}


/* Longest name */
unsigned rhtsuite_maxn (rhtsuite_t * argv [])
{
  unsigned n = 0;
  while (argv && * argv)
    {
      n = RMAX (n, strlen ((* argv) -> name));
      argv ++;
    }
  return n;
}


/* Longest description */
unsigned rhtsuite_maxd (rhtsuite_t * argv [])
{
  unsigned n = 0;
  while (argv && * argv)
    {
      n = RMAX (n, strlen ((* argv) -> description));
      argv ++;
    }
  return n;
}


/* Run the Test Suite included in suite[] */
void rhtsuite_run (rhtsuite_t * suite [], unsigned argc, void * argv [])
{
  unsigned maxn = rhtsuite_maxn (suite);
  unsigned n    = digits (arrlen (suite));
  unsigned seq  = 0;

  while (suite && * suite)
    rhtsuite_run_one (* suite ++, argc, argv, n, ++ seq, maxn);
}


/* Clear the results of a Test Suite execution */
void rhtsuite_clear_results (rhtsuite_t * suite [])
{
  while (suite && * suite)
    arrclear ((* suite ++) -> results, rmelapsed);
}


/* Sort the results of a Test Suite execution by less avg time */
void rhtsuite_sort_results (rhtsuite_t * suite [])
{
  while (suite && * suite)
    {
      (* suite) -> results = arrsort ((* suite) -> results, sort_by_less_avg, relapsed_t);
      suite ++;
    }
}


/* Print all the builtin Test Suite */
void rhtsuite_print_all (void)
{
  unsigned maxn = rhtsuite_all_maxn ();
  unsigned i;

  rhtsuite_print_header (maxn);
  for (i = 0; i < rhtsuite_no (); i ++)
    rhtsuite_print_one (rhtsuite_find_at (i), i + 1, maxn);
}
