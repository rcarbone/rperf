/*
 * Utilities to access the Suite defined in static tables.
 */


/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Project headers */
#include "rsuite.h"
#define NEED_RTREE_TYPEDEF
#include "rtree-api.h"
#include "rwall.h"
#include "sargv.h"
#include "support.h"
#include "rctype.h"


#define SEP  '|'


static void rsuite_print_header (unsigned maxn)
{
  printf (" # %c %-*.*s %c %s %c %s\n", SEP, maxn, maxn, "Name", SEP, "Id", SEP, "Description");
  printf ("--- %s %c %s %c %s\n", "--------", SEP, "--", SEP, "----------------------");
}


/* Return the handle of the first item */
rsuite_t * rsuite_first (rsuite_t * argv)
{
  return & argv [0];
}


/* Return the handle of the last item */
rsuite_t * rsuite_last (unsigned argc, rsuite_t * argv)
{
  return & argv [argc - 1];
}


/* Lookup for an item by index [0 - argc-1] */
rsuite_t * rsuite_at (unsigned argc, rsuite_t * argv, unsigned at)
{
  return at < argc ? & argv [at] : NULL;
}


/* Return the handle of an item randomly generated */
rsuite_t * rsuite_rnd (unsigned argc, rsuite_t * argv)
{
  return & argv [rrand (argc)];
}


/* Lookup for an item by its unique name */
rsuite_t * rsuite_name (unsigned argc, rsuite_t * argv, char * name)
{
  unsigned i;
  if (name)
    for (i = 0; i < argc; i ++)
      if (! strcmp (argv [i] . name, name))
	return & argv [i];
  return NULL;
}


/* Check if 'id' is a legal identifier (numeric in the range [1-argc] or alphanumeric */
rsuite_t * rsuite_valid (unsigned argc, rsuite_t * argv, char * id)
{
  return isnumeric (id) && atoi (id) ? rsuite_at (argc, argv, atoi (id) - 1) : rsuite_name (argc, argv, id);
}


/* Evaluate the number of characters in the longest name */
unsigned rsuite_maxn (unsigned argc, rsuite_t * argv)
{
  unsigned n = 0;
  unsigned i;
  for (i = 0; i < argc; i ++)
    n = RMAX (n, strlen (argv [i] . name));
  return n;
}


/* Evaluate the number of characters in the longest description */
unsigned rsuite_maxd (unsigned argc, rsuite_t * argv)
{
  unsigned n = 0;
  unsigned i;
  for (i = 0; i < argc; i ++)
    n = RMAX (n, strlen (argv [i] . description));
  return n;
}


/* Print an item */
void rsuite_print (rsuite_t * test, unsigned n, unsigned maxn)
{
  printf ("%3d%c %-*.*s %c%3d %c %s\n", n, SEP, maxn, maxn, test -> name, SEP, n, SEP, test -> description);
}


/* Run an item */
void rsuite_run (rsuite_t * suite, unsigned argc, void * argv, unsigned digits, unsigned seq, unsigned maxn)
{
  rtime_t elapsed;

  print_dots (suite -> name, "Running", digits, seq, maxn);

  elapsed = suite -> func (argc, argv);
  if (elapsed)
    printf ("Ok - %s\n", ns2a (elapsed));
  else
    printf ("No\n");
}


/* Return all the handles in an array */
rsuite_t ** rsuite_handles (unsigned argc, rsuite_t * argv)
{
  rsuite_t ** handles = NULL;
  unsigned i;
  for (i = 0; i < argc; i ++)
    handles = arrmore (handles, & argv [i], rsuite_t);
  return handles;
}


/* Return all the handles in a shuffled array */
rsuite_t ** rsuite_shuffle (unsigned argc, rsuite_t * argv)
{
  return (rsuite_t **) varnd (argc, (void **) rsuite_handles (argc, argv));
}


/* Return all the names in an array */
char ** rsuite_names (unsigned argc, rsuite_t * argv)
{
  char ** names = NULL;
  unsigned i;
  for (i = 0; i < argc; i ++)
    names = argsmore (names, argv [i] . name);
  return names;
}


/* Print all items */
void rsuite_prints (unsigned argc, rsuite_t * argv)
{
  unsigned maxn = rsuite_maxn (argc, argv);
  unsigned i;

  rsuite_print_header (maxn);
  for (i = 0; i < argc; i ++)
    rsuite_print (rsuite_at (argc, argv, i), i + 1, maxn);
}


/* Clear the results */
void rsuite_clear_results (rsuite_t * suite [])
{
  while (suite && * suite)
    arrclear ((* suite ++) -> results, rmelapsed);
}


/* Sort the results by less avg time */
void rsuite_sort_results (rsuite_t * suite [])
{
  while (suite && * suite)
    {
      (* suite) -> results = arrsort ((* suite) -> results, sort_by_less_avg, relapsed_t);
      suite ++;
    }
}


/* Evaluate the number of characters in the longest name */
unsigned rsuite_MAXN (rsuite_t * suite [])
{
  unsigned n = 0;
  while (suite && * suite)
    {
      n = RMAX (n, strlen ((* suite) -> name));
      suite ++;
    }
  return n;
}


/* Evaluate the number of characters in the longest description */
unsigned rsuite_MAXD (rsuite_t * suite [])
{
  unsigned n = 0;
  while (suite && * suite)
    {
      n = RMAX (n, strlen ((* suite) -> description));
      suite ++;
    }
  return n;
}


char ** rsuite_NAMES (rsuite_t * suite [])
{
  char ** names = NULL;
  while (suite && * suite)
    names = argsmore (names, (* suite ++) -> name);
  return names;
}
