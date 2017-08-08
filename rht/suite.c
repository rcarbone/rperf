/*
 * All the Test Suite are here, grouped together in a static table.
 *
 * Each implementation of a single Test Suite operates with 'n' different unique keys.
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


/* The local identifiers for the Test Suite to run (indexed by keys) */
typedef enum
{
  RSUITE_GROW    = 0x01,  /* Populate an empty container                        */
  RSUITE_HIT     = 0x02,  /* Populate and search all existing items one-by-one  */
  RSUITE_MISS    = 0x03,  /* Populate and search for non existing items         */
  RSUITE_DELETE  = 0x04,  /* Populate and delete all existing items one-by-one  */
  RSUITE_REPLACE = 0x05,  /* Populate and replace all existing items one-by-one */
  RSUITE_KBENCH  = 0x06,  /* Delete if found, add otherwise items one-by-one    */

} rsuite_id_t;


/* The implementation elsewhere defined */
rsuite_f rsuite_grow;
rsuite_f rsuite_hit;
rsuite_f rsuite_miss;
rsuite_f rsuite_delete;
rsuite_f rsuite_replace;
rsuite_f rsuite_kbench;


/* All the Test Suite in a static array */
static rtest_t rsuite_builtins [] =
{
  { RSUITE_GROW,    "grow",    "Add items to an empty container", NULL, rsuite_grow    },
  { RSUITE_HIT,     "hit",     "Find existing items",             NULL, rsuite_hit     },
  { RSUITE_MISS,    "miss",    "Search non existing items",       NULL, rsuite_miss    },
  { RSUITE_DELETE,  "delete",  "Delete existing items",           NULL, rsuite_delete  },
  { RSUITE_REPLACE, "replace", "Replace existing items",          NULL, rsuite_replace },
  { RSUITE_KBENCH,  "kbench",  "Delete if found, add otherwise",  NULL, rsuite_kbench  },
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
  unsigned i;
  rtest_t ** all = NULL;
  for (i = 0; i < RSUITE_NO; i ++)
    all = arrmore (all, & rsuite_builtins [i], rtest_t);
  return all;
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
