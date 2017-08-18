/*
 * All the Unit Tests are here, grouped together in a static table.
 *
 * Each implementation of a single Unit Test operates with 'n' different elements
 * and all are based on the assert(3) to check for assertions.
 */


/* System headers */
#include <assert.h>

/* Project headers */
#define NEED_RL_TYPEDEF
#include "rl.h"

#include "sargv.h"
#include "rwall.h"
#include "rctype.h"
#include "support.h"
#include "rltest.h"
#include "rlunit.h"


/* The implementation elsewhere defined */
rlunit_f alloc_free;
rlunit_f alloc_prepend_free;
rlunit_f alloc_append_free;
rlunit_f alloc_clear_free;
rlunit_f alloc_count_free;
rlunit_f alloc_found_free;
rlunit_f alloc_miss_free;
rlunit_f alloc_delete_free;
rlunit_f alloc_missed_free;
rlunit_f alloc_iterate_free;


/* All the Unit Tests in a static table */
static rlunit_t builtins [] =
{
  { "make",    "Allocate and free an empty container", alloc_free         },
  { "prepend", "Add elements at the head of the list", alloc_prepend_free },
  { "append",  "Add elements at the tail of the list", alloc_append_free  },
  { "clear",   "Add elements and clear",               alloc_clear_free   },
  { "count",   "Add elements and count",               alloc_count_free   },
  { "found",   "Search for existent one-by-one",       alloc_found_free   },
  { "miss",    "Search for non-existent one-by-one",   alloc_miss_free    },
  { "delete",  "Delete existent one-by-one",           alloc_delete_free  },
  { "missed",  "Delete non-existent one-by-one",       alloc_missed_free  },
  { "foreach", "Iterate over existent one-by-one",     alloc_iterate_free },
};
#define RLUNIT_NO (sizeof (builtins) / sizeof (* builtins))


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


static void rlunit_run_one (rlunit_t * rlunit, unsigned items, unsigned n, unsigned seq, unsigned maxn)
{
  unsigned t;

  print_dots (rlunit -> name, "Running", n, seq, maxn);

  t = rlunit -> unit (items);
  if (t == items)
    printf ("Ok\n");
  else
    printf ("No\n");
}


static void rlunit_print_header (unsigned maxn)
{
  printf (" # %c %-*.*s %c %s %c %s\n", SEP, maxn, maxn, "Name", SEP, "Id", SEP, "Description");
  printf ("--- %s %c %s %c %s\n", "--------", SEP, "--", SEP, "----------------------");
}


static void rlunit_print_one (rlunit_t * rlunit, unsigned n, unsigned maxn)
{
  if (rlunit)
    printf ("%3d%c %-*.*s %c%3d %c %s\n", n, SEP, maxn, maxn, rlunit -> name, SEP, n, SEP, rlunit -> description);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Return the # of Unit Tests */
unsigned rlunit_no (void)
{
  return RLUNIT_NO;
}


/* Return the handle of the first Unit Test */
rlunit_t * rlunit_first (void)
{
  return & builtins [0];
}


/* Return the handle of the last Unit Test */
rlunit_t * rlunit_last (void)
{
  return & builtins [RLUNIT_NO - 1];
}


/* Return the handle of the next Unit Test */
rlunit_t * rlunit_next (void)
{
  static unsigned i = 0;
  return & builtins [i ++ % RLUNIT_NO];
}


/* Return the handle of the next Unit Test randomly generated */
rlunit_t * rlunit_next_rnd (void)
{
  return & builtins [rrand (RLUNIT_NO)];
}


/* Looukup for an Unit Test by index */
rlunit_t * rlunit_find_at (unsigned i)
{
  return i < RLUNIT_NO ? & builtins [i] : NULL;
}


/* Looukup for an Unit Test by its unique id */
rlunit_t * rlunit_find_by_id (unsigned id)
{
  return id > 0 && id <= RLUNIT_NO ? & builtins [id - 1] : NULL;
}


/* Looukup for an Unit Test by its unique name */
rlunit_t * rlunit_find_by_name (char * name)
{
  unsigned i;
  if (name)
    for (i = 0; i < RLUNIT_NO; i ++)
      if (! strcmp (builtins [i] . name, name))
	return & builtins [i];
  return NULL;
}


/* Return all the Unit Test names in an array in the same order they were defined */
char ** rlunit_names (void)
{
  unsigned i;
  char ** all = NULL;
  for (i = 0; i < RLUNIT_NO; i ++)
    all = argsmore (all, builtins [i] . name);
  return all;
}


/* Return all the Unit Tests handles in an array in the same order they were defined */
rlunit_t ** rlunit_all (void)
{
  unsigned i;
  rlunit_t ** all = NULL;
  for (i = 0; i < RLUNIT_NO; i ++)
    all = arrmore (all, & builtins [i], rlunit_t);
  return all;
}


/* Return all the Unit Tests in an array in the same order they were defined starting at given offset */
rlunit_t ** rlunit_all_n (unsigned n)
{
  rlunit_t ** all;
  unsigned i;

  n %= RLUNIT_NO;

  for (i = 0; i < n; i ++)
    rlunit_next ();

  all = arrmore (NULL, rlunit_next (), rlunit_t);
  while (arrlen (all) != rlunit_no ())
    all = arrmore (all, rlunit_next (), rlunit_t);
  return all;
}


rlunit_t ** rlunit_all_rnd (void)
{
  return (rlunit_t **) varnd (rlunit_no (), (void **) rlunit_all ());
}


/* Longest name */
unsigned rlunit_all_maxn (void)
{
  unsigned n = 0;
  unsigned i;
  for (i = 0; i < RLUNIT_NO; i ++)
    n = RMAX (n, strlen (builtins [i] . name));
  return n;
}


/* Longest name */
unsigned rlunit_maxn (rlunit_t * argv [])
{
  unsigned n = 0;
  while (argv && * argv)
    {
      n = RMAX (n, strlen ((* argv) -> name));
      argv ++;
    }
  return n;
}


rlunit_t * rlunit_valid (char * id)
{
  return isnumeric (id) && atoi (id) ? rlunit_find_at (atoi (id) - 1) : rlunit_find_by_name (id);
}


/* Run the Unit Tests included in argv[] */
void rlunit_run (rlunit_t * argv [], unsigned items)
{
  unsigned maxn = rlunit_maxn (argv);
  unsigned n    = digits (arrlen (argv));
  unsigned seq  = 0;

  while (argv && * argv)
    rlunit_run_one (* argv ++, items, n, ++ seq, maxn);
}


/* -=-=-=-=-=-=-= API -=-=-=-=-=-=-= */


void rlunit_print_no (void)
{
  printf ("Built-in Units Tests : %u\n", rlunit_no ());
}


/* Print the Unit Tests included in argv[] */
void rlunit_print (rlunit_t * argv [])
{
  unsigned maxn = rlunit_maxn (argv);
  unsigned i = 0;

  rlunit_print_header (maxn);
  while (argv && * argv)
    rlunit_print_one (* argv ++, ++ i, maxn);
}


/* Print the all the builtin Unit Tests */
void rlunit_print_all (void)
{
  unsigned maxn = rlunit_all_maxn ();
  unsigned i;

  rlunit_print_header (maxn);
  for (i = 0; i <= rlunit_no (); i ++)
    rlunit_print_one (rlunit_find_at (i), i + 1, maxn);
}

