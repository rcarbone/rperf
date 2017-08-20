/*
 * All the Test Suite are here, grouped together in a static table.
 *
 * Each implementation of a single Test Suite operates with 'n' different elements.
 */


/* Project headers */
#define NEED_RL_TYPEDEF
#include "rl.h"
#include "sargv.h"
#include "rwall.h"
#include "rctype.h"
#include "support.h"
#include "elems.h"
#include "rlsuite.h"


#if defined(ROCCO)
/* The implementations elsewhere defined */
rlsuite_f rlsuite_grow_head;
rlsuite_f rlsuite_grow_tail;
#endif /* ROCCO */


/* All the Test Suite in a static table */
static rlsuite_t builtins [] =
{
  { "grow-head", "Populate an empty list", rlsuite_grow_head },
  { "grow-tail", "Populate an empty list", rlsuite_grow_tail },
};
#define RLSUITE_NO (sizeof (builtins) / sizeof (* builtins))


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


static void rlsuite_run_one (rlsuite_t * test, unsigned argc, relem_t * argv [], unsigned n, unsigned seq, unsigned maxn)
{
  rtime_t elapsed;

  print_dots (test -> name, "Running", n, seq, maxn);

  elapsed = test -> func (argc, argv);
  if (elapsed)
    printf ("Ok - %s\n", ns2a (elapsed));
  else
    printf ("No\n");
}


static void rlsuite_print_header (unsigned maxn)
{
  printf (" # %c %-*.*s %c %s %c %s\n", SEP, maxn, maxn, "Name", SEP, "Id", SEP, "Description");
  printf ("--- %s %c %s %c %s\n", "--------", SEP, "--", SEP, "----------------------");
}


static void rlsuite_print_one (rlsuite_t * test, unsigned n, unsigned maxn)
{
  if (test)
    printf ("%3d%c %-*.*s %c%3d %c %s\n", n, SEP, maxn, maxn, test -> name, SEP, n, SEP, test -> description);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* Return the # of Test Suite */
unsigned rlsuite_no (void)
{
  return RLSUITE_NO;
}


/* Return the handle of the first Test Suite */
rlsuite_t * rlsuite_first (void)
{
  return & builtins [0];
}


/* Return the handle of the last Test Suite */
rlsuite_t * rlsuite_last (void)
{
  return & builtins [RLSUITE_NO - 1];
}


/* Return the handle of the next Test Suite */
rlsuite_t * rlsuite_next (void)
{
  static unsigned i = 0;
  return & builtins [i ++ % RLSUITE_NO];
}


/* Return the handle of the next Test Suite randomly generated */
rlsuite_t * rlsuite_next_rnd (void)
{
  return & builtins [rrand (RLSUITE_NO)];
}


/* Looukup for a Test Suite by index */
rlsuite_t * rlsuite_find_at (unsigned i)
{
  return i < RLSUITE_NO ? & builtins [i] : NULL;
}


/* Looukup for a Test Suite by its unique id */
rlsuite_t * rlsuite_find_by_id (unsigned id)
{
  return id > 0 && id <= RLSUITE_NO ? & builtins [id - 1] : NULL;
}


/* Looukup for a Test Suite by its unique name */
rlsuite_t * rlsuite_find_by_name (char * name)
{
  unsigned i;
  for (i = 0; name && i < RLSUITE_NO; i ++)
    if (! strcmp (builtins [i] . name, name))
      return & builtins [i];
  return NULL;
}


/* Return all the Test Suite names in an array in the same order they were defined */
char ** rlsuite_all_names (void)
{
  char ** all = NULL;
  unsigned i;
  for (i = 0; i < RLSUITE_NO; i ++)
    all = argsmore (all, builtins [i] . name);
  return all;
}


/* Return all the Test Suite in an array in the same order they were defined */
rlsuite_t ** rlsuite_all (void)
{
  rlsuite_t ** all = NULL;
  unsigned i;
  for (i = 0; i < RLSUITE_NO; i ++)
    all = arrmore (all, & builtins [i], rlsuite_t);
  return all;
}


/* Longest name */
unsigned rlsuite_all_maxn (void)
{
  unsigned n = 0;
  unsigned i;
  for (i = 0; i < RLSUITE_NO; i ++)
    n = RMAX (n, strlen (builtins [i] . name));
  return n;
}


/* Return all the Test Suite in an array in the same order they were defined starting at given offset */
rlsuite_t ** rlsuite_all_n (unsigned n)
{
  rlsuite_t ** all;
  unsigned i;

  n %= RLSUITE_NO;

  for (i = 0; i < n; i ++)
    rlsuite_next ();

  all = arrmore (NULL, rlsuite_next (), rlsuite_t);
  while (arrlen (all) != rlsuite_no ())
    all = arrmore (all, rlsuite_next (), rlsuite_t);
  return all;
}


rlsuite_t ** rlsuite_all_rnd (void)
{
  return (rlsuite_t **) varnd (rlsuite_no (), (void **) rlsuite_all ());
}


/* Print the all the builtin Test Suite */
void rlsuite_print_all (void)
{
  unsigned maxn = rlsuite_all_maxn ();
  unsigned i;

  rlsuite_print_header (maxn);
  for (i = 0; i <= rlsuite_no (); i ++)
    rlsuite_print_one (rlsuite_find_at (i), i + 1, maxn);
}


/* Return the Test Suite names in the same order they were defined in the table */
char ** rlsuite_names (rlsuite_t * suite [])
{
  char ** all = NULL;
  while (suite && * suite)
    all = argsmore (all, (* suite ++) -> name);
  return all;
}


rlsuite_t * rlsuite_valid (char * id)
{
  return isnumeric (id) && atoi (id) ? rlsuite_find_at (atoi (id) - 1) : rlsuite_find_by_name (id);
}


/* Longest name */
unsigned rlsuite_maxn (rlsuite_t * argv [])
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
unsigned rlsuite_maxd (rlsuite_t * argv [])
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
void rlsuite_run (rlsuite_t * suite [], unsigned argc, relem_t * argv [])
{
  unsigned maxn = rlsuite_maxn (suite);
  unsigned n    = digits (arrlen (suite));
  unsigned seq  = 0;

  while (suite && * suite)
    rlsuite_run_one (* suite ++, argc, argv, n, ++ seq, maxn);
}


/* Clear the results of a Test Suite execution */
void rlsuite_clear_results (rlsuite_t * suite [])
{
  while (suite && * suite)
    arrclear ((* suite ++) -> results, rmelapsed);
}


/* Sort the results of a Test Suite execution by less avg time */
void rlsuite_sort_results (rlsuite_t * suite [])
{
  while (suite && * suite)
    {
      (* suite) -> results = arrsort ((* suite) -> results, sort_by_less_avg, relapsed_t);
      suite ++;
    }
}
