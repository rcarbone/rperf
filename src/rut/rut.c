/*
 * Utilities to access the Unit Tests defined in static tables.
 */


/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Project headers */
#include "rut.h"
#include "rwall.h"
#include "sargv.h"
#include "support.h"
#include "rctype.h"


#define SEP  '|'


static void rut_print_header (unsigned maxn)
{
  printf (" # %c %-*.*s %c %s %c %s\n", SEP, maxn, maxn, "Name", SEP, "Id", SEP, "Description");
  printf ("--- %s %c %s %c %s\n", "--------", SEP, "--", SEP, "----------------------");
}


/* Return the handle of the first item */
rut_t * rut_first (rut_t * argv)
{
  return & argv [0];
}


/* Return the handle of the last item */
rut_t * rut_last (unsigned argc, rut_t * argv)
{
  return & argv [argc - 1];
}


/* Lookup for an item by index [0 - argc-1] */
rut_t * rut_at (unsigned argc, rut_t * argv, unsigned at)
{
  return at < argc ? & argv [at] : NULL;
}


/* Return the handle of an item randomly generated */
rut_t * rut_rnd (unsigned argc, rut_t * argv)
{
  return & argv [rrand (argc)];
}


/* Lookup for an item by its unique name */
rut_t * rut_name (unsigned argc, rut_t * argv, char * name)
{
  unsigned i;
  if (name)
    for (i = 0; i < argc; i ++)
      if (! strcmp (argv [i] . name, name))
	return & argv [i];
  return NULL;
}


/* Check if 'id' is a legal identifier (numeric in the range [1-argc] or alphanumeric */
rut_t * rut_valid (unsigned argc, rut_t * argv, char * id)
{
  return isnumeric (id) && atoi (id) ? rut_at (argc, argv, atoi (id) - 1) : rut_name (argc, argv, id);
}


/* Evaluate the number of characters in the longest name */
unsigned rut_maxn (unsigned argc, rut_t * argv)
{
  unsigned n = 0;
  unsigned i;
  for (i = 0; i < argc; i ++)
    n = RMAX (n, strlen (argv [i] . name));
  return n;
}


/* Print an item */
void rut_print (rut_t * ut, unsigned indent, unsigned maxn)
{
  printf ("%*.*sName : %-*.*s (%s)\n", indent, indent, " ", maxn, maxn, ut -> name, ut -> description);
}


/* Run an item */
void rut_run (rut_t * unit, unsigned items, unsigned n, unsigned seq, unsigned maxn)
{
  unsigned t;

  print_dots (unit -> name, "Running", n, seq, maxn);

  t = unit -> func (items);
  if (t == items)
    printf ("Ok\n");
  else
    printf ("No\n");
}


/* Return all the handles in an array */
rut_t ** rut_handles (unsigned argc, rut_t * argv)
{
  rut_t ** handles = NULL;
  unsigned i;
  for (i = 0; i < argc; i ++)
    handles = arrmore (handles, & argv [i], rut_t);
  return handles;
}


/* Return all the handles in a shuffled array */
rut_t ** rut_shuffle (unsigned argc, rut_t * argv)
{
  return (rut_t **) varnd (argc, (void **) rut_handles (argc, argv));
}


/* Return all the names in an array */
char ** rut_names (unsigned argc, rut_t * argv)
{
  char ** names = NULL;
  unsigned i;
  for (i = 0; i < argc; i ++)
    names = argsmore (names, argv [i] . name);
  return names;
}


/* Print all the items */
void rut_prints (unsigned argc, rut_t * argv)
{
  unsigned mx = rut_maxn (argc, argv);
  unsigned i;

  rut_print_header (mx);
  for (i = 0; i < argc; i ++)
    rut_print (rut_at (argc, argv, i), 2, mx);
}


#if defined(ROCCO)
/* Run all the Unit Tests */
void rut_runs (unsigned argc, rut_t * argv [], unsigned items)
{
  unsigned maxn = rut_maxn (argc, argv);
  unsigned n    = digits (argc);
  unsigned seq  = 0;

  while (argv && * argv)
    rut_run_one (* argv ++, items, n, ++ seq, maxn);
}
#endif /* ROCCO */
