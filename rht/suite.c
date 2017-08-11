/*
 * All the Test Suite are here, grouped together in a static table.
 *
 * Each implementation of a single Test Suite operates with 'n' different unique keys
 * (except kbench which requires a dataset of non-unique keys).
 */


/* Project headers */
typedef struct rht rht_t;
#include "rht.h"
#include "sargs.h"
#include "rwall.h"
#include "rctype.h"
#include "support.h"
#include "datasets.h"
#include "rtest.h"
#include "rsuite.h"


/* All the Test Suite in a static array */
static rtest_t rsuite_builtins [] =
{
  { RSUITE_GROW_SEQ,    "grow_seq",    "Populate an empty container (sequential keys)",           NULL, rsuite_grow_seq    },
  { RSUITE_GROW_RND,    "grow_rnd",    "Populate an empty container (random keys)",               NULL, rsuite_grow_rnd    },
  { RSUITE_HIT_SEQ,     "hit_seq",     "Search one-by-one all existing items (sequential keys)",  NULL, rsuite_hit_seq     },
  { RSUITE_HIT_RND,     "hit_rnd",     "Search one-by-one all existing items (random keys)",      NULL, rsuite_hit_rnd     },
  { RSUITE_MISS_SEQ,    "miss_seq",    "Search for non existing items (sequential keys)",         NULL, rsuite_miss_seq    },
  { RSUITE_MISS_RND,    "miss_rnd",    "Search for non existing items (random keys)",             NULL, rsuite_miss_rnd    },
  { RSUITE_DELETE_SEQ,  "delete_seq",  "Delete one-by-one all existing items (sequential keys)",  NULL, rsuite_delete_seq  },
  { RSUITE_DELETE_RND,  "delete_rnd",  "Delete one-by-one all existing items (random keys)",      NULL, rsuite_delete_rnd  },
  { RSUITE_REPLACE_SEQ, "replace_seq", "Replace one-by-one all existing items (sequential keys)", NULL, rsuite_replace_seq },
  { RSUITE_REPLACE_RND, "replace_rnd", "Replace one-by-one all existing items (random keys)",     NULL, rsuite_replace_rnd },
  { RSUITE_KBENCH,      "kbench",      "Delete if found, add otherwise (non-unique keys)",        NULL, rsuite_kbench      },
};
#define RSUITE_NO (sizeof (rsuite_builtins) / sizeof (* rsuite_builtins))


static void rsuite_run_one (rtest_t * rtest, unsigned argc, robj_t * argv [], unsigned n, unsigned seq, unsigned maxn)
{
  rtime_t elapsed;

  print_dots (rtest -> name, "Running", n, seq, maxn);

  elapsed = rtest -> suite (argc, argv);
  if (elapsed)
    printf ("Ok - %s\n", ns2a (elapsed));
  else
    printf ("No\n");
}


/* Return the # of Test Suite */
unsigned rsuite_no (void)
{
  return RSUITE_NO;
}


/* Return the handle of the first Test Suite */
rtest_t * rsuite_first (void)
{
  return & rsuite_builtins [0];
}


/* Return the handle of the last Test Suite */
rtest_t * rsuite_last (void)
{
  return & rsuite_builtins [RSUITE_NO - 1];
}


/* Return the handle of the next Test Suite */
rtest_t * rsuite_next (void)
{
  static unsigned i = 0;
  return & rsuite_builtins [i ++ % RSUITE_NO];
}


/* Return the handle of the next Test Suite randomly generated */
rtest_t * rsuite_next_rnd (void)
{
  return & rsuite_builtins [rrand (RSUITE_NO)];
}


/* Looukup for a Test Suite by index */
rtest_t * rsuite_find_at (unsigned i)
{
  return i < RSUITE_NO ? & rsuite_builtins [i] : NULL;
}


/* Looukup for a Test Suite by its unique id */
rtest_t * rsuite_find_by_id (unsigned id)
{
  unsigned i;
  for (i = 0; i < RSUITE_NO; i ++)
    if (rsuite_builtins [i] . id == id)
      return & rsuite_builtins [i];
  return NULL;
}


/* Looukup for a Test Suite by its unique name */
rtest_t * rsuite_find_by_name (char * name)
{
  unsigned i;
  for (i = 0; name && i < RSUITE_NO; i ++)
    if (! strcmp (rsuite_builtins [i] . name, name))
      return & rsuite_builtins [i];
  return NULL;
}


/* Return all the Test Suite names in an array in the same order they were defined */
char ** rsuite_all_names (void)
{
  char ** all = NULL;
  unsigned i;
  for (i = 0; i < RSUITE_NO; i ++)
    all = argsmore (all, rsuite_builtins [i] . name);
  return all;
}


/* Return all the Test Suite in an array in the same order they were defined */
rtest_t ** rsuite_all (void)
{
  rtest_t ** all = NULL;
  unsigned i;
  for (i = 0; i < RSUITE_NO; i ++)
    all = arrmore (all, & rsuite_builtins [i], rtest_t);
  return all;
}


/* Longest name */
unsigned rsuite_all_maxn (void)
{
  unsigned n = 0;
  unsigned i;
  for (i = 0; i < RSUITE_NO; i ++)
    n = RMAX (n, strlen (rsuite_builtins [i] . name));
  return n;
}


/* Return all the Test Suite in an array in the same order they were defined starting at given offset */
rtest_t ** rsuite_all_n (unsigned n)
{
  rtest_t ** all;
  unsigned i;

  n %= RSUITE_NO;

  for (i = 0; i < n; i ++)
    rsuite_next ();

  all = arrmore (NULL, rsuite_next (), rtest_t);
  while (arrlen (all) != rsuite_no ())
    all = arrmore (all, rsuite_next (), rtest_t);
  return all;
}


rtest_t ** rsuite_all_rnd (void)
{
  return (rtest_t **) varnd (rsuite_no (), (void **) rsuite_all ());
}


/* Return the Test Suite names in the same order they were defined in the table */
char ** rsuite_names (rtest_t * suite [])
{
  char ** all = NULL;
  while (suite && * suite)
    all = argsmore (all, (* suite ++) -> name);
  return all;
}


rtest_t * rsuite_valid (char * id)
{
  return isnumeric (id) && atoi (id) ? rsuite_find_at (atoi (id) - 1) : rsuite_find_by_name (id);
}


/* Longest name */
unsigned rsuite_maxn (rtest_t * argv [])
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
unsigned rsuite_maxd (rtest_t * argv [])
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
void rsuite_run (rtest_t * suite [], unsigned argc, robj_t * argv [])
{
  unsigned maxn = rsuite_maxn (suite);
  unsigned n    = digits (arrlen (suite));
  unsigned seq  = 0;

  while (suite && * suite)
    rsuite_run_one (* suite ++, argc, argv, n, ++ seq, maxn);
}


/* Clear the results of a Test Suite execution */
void rsuite_clear_results (rtest_t * suite [])
{
  while (suite && * suite)
    arrclear ((* suite ++) -> results, rmelapsed);
}


/* Sort the results of a Test Suite execution by less avg time */
void rsuite_sort_results (rtest_t * suite [])
{
  while (suite && * suite)
    {
      (* suite) -> results = arrsort ((* suite) -> results, sort_by_less_avg, relapsed_t);
      suite ++;
    }
}
