/*
 * All the Unit Tests are here, grouped together in a static table.
 *
 * Each implementation of a single Unit Test operates with 'n' different unique keys
 * and all are based on the assert(3) to check for assertions.
 */


/* System headers */
#include <assert.h>

/* Project headers */
#define NEED_RHT_TYPEDEF
#include "rht-api.h"
#include "sargv.h"
#include "rwall.h"
#include "rctype.h"
#include "support.h"
#include "rht-test.h"
#include "rht-unit.h"


/* The implementation elsewhere defined */
rhtunit_f alloc_free;
rhtunit_f alloc_add_free;
rhtunit_f alloc_add_clear_free;
rhtunit_f alloc_add_count_free;
rhtunit_f alloc_add_found_free;
rhtunit_f alloc_add_notfound_free;
rhtunit_f alloc_add_delete_free;
rhtunit_f alloc_add_missed_free;
rhtunit_f alloc_add_iterate_free;
rhtunit_f alloc_add_keys_free;
rhtunit_f alloc_add_vals_free;


/* All the Unit Tests in a static table */
static rhtunit_t builtins [] =
{
  { "make",    "Allocate and free an empty container", alloc_free,              },
  { "fill",    "Add items to an empty container",      alloc_add_free,          },
  { "clear",   "Add items and clear",                  alloc_add_clear_free,    },
  { "count",   "Add items and count",                  alloc_add_count_free,    },
  { "found",   "Find existing items",                  alloc_add_found_free,    },
  { "miss",    "Search non existing items",            alloc_add_notfound_free, },
  { "delete",  "Delete existing items",                alloc_add_delete_free,   },
  { "missed",  "Delete non existing items",            alloc_add_missed_free,   },
  { "foreach", "Iterate existing items",               alloc_add_iterate_free,  },
  { "keys",    "Get all the keys",                     alloc_add_keys_free,     },
  { "vals",    "Get all the values",                   alloc_add_vals_free,     },
};
#define RHTUNIT_NO (sizeof (builtins) / sizeof (* builtins))


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


static void rhtunit_run_one (rhtunit_t * unit, unsigned items, unsigned n, unsigned seq, unsigned maxn)
{
  unsigned t;

  print_dots (unit -> name, "Running", n, seq, maxn);

  t = unit -> func (items);
  if (t == items)
    printf ("Ok\n");
  else
    printf ("No\n");
}


static void rhtunit_print_header (unsigned maxn)
{
  printf (" # %c %-*.*s %c %s %c %s\n", SEP, maxn, maxn, "Name", SEP, "Id", SEP, "Description");
  printf ("--- %s %c %s %c %s\n", "------------", SEP, "--", SEP, "----------------------");
}


static void rhtunit_print_one (rhtunit_t * runit, unsigned n, unsigned maxn)
{
  printf ("%3d%c %-*.*s %c%3d %c %s\n", n, SEP, maxn, maxn, runit -> name, SEP, n, SEP, runit -> description);
}


/* -=-=-=-=-=-=-= API -=-=-=-=-=-=-= */


/* Return the # of Unit Tests */
unsigned rhtunit_no (void)
{
  return RHTUNIT_NO;
}


/* Return the handle of the first Unit Test */
rhtunit_t * rhtunit_first (void)
{
  return & builtins [0];
}


/* Return the handle of the last Unit Test */
rhtunit_t * rhtunit_last (void)
{
  return & builtins [RHTUNIT_NO - 1];
}


/* Return the handle of the next Unit Test */
rhtunit_t * rhtunit_next (void)
{
  static unsigned i = 0;
  return & builtins [i ++ % RHTUNIT_NO];
}


/* Return the handle of the next Unit Test randomly generated */
rhtunit_t * rhtunit_next_rnd (void)
{
  return & builtins [rrand (RHTUNIT_NO)];
}


/* Looukup for an Unit Test by index */
rhtunit_t * rhtunit_find_at (unsigned i)
{
  return i < RHTUNIT_NO ? & builtins [i] : NULL;
}


/* Looukup for an Unit Test by its unique id */
rhtunit_t * rhtunit_find_by_id (unsigned id)
{
  return id > 0 && id <= RHTUNIT_NO ? & builtins [id - 1] : NULL;
}


/* Looukup for an Unit Test by its unique name */
rhtunit_t * rhtunit_find_by_name (char * name)
{
  unsigned i;
  for (i = 0; name && i < RHTUNIT_NO; i ++)
    if (! strcmp (builtins [i] . name, name))
      return & builtins [i];
  return NULL;
}


/* Return all the Unit Test names in an array in the same order they were defined */
char ** rhtunit_names (void)
{
  unsigned i;
  char ** all = NULL;
  for (i = 0; i < RHTUNIT_NO; i ++)
    all = argsmore (all, builtins [i] . name);
  return all;
}


/* Return all the Unit Tests handles in an array in the same order they were defined */
rhtunit_t ** rhtunit_all (void)
{
  unsigned i;
  rhtunit_t ** all = NULL;
  for (i = 0; i < RHTUNIT_NO; i ++)
    all = arrmore (all, & builtins [i], rhtunit_t);
  return all;
}


/* Return all the Unit Tests handles in an array in the same order they were defined starting at given offset */
rhtunit_t ** rhtunit_all_n (unsigned n)
{
  rhtunit_t ** all;
  unsigned i;

  n %= RHTUNIT_NO;

  for (i = 0; i < n; i ++)
    rhtunit_next ();

  all = arrmore (NULL, rhtunit_next (), rhtunit_t);
  while (arrlen (all) != rhtunit_no ())
    all = arrmore (all, rhtunit_next (), rhtunit_t);
  return all;
}


rhtunit_t ** rhtunit_all_rnd (void)
{
  return (rhtunit_t **) varnd (rhtunit_no (), (void **) rhtunit_all ());
}


rhtunit_t * rhtunit_valid (char * id)
{
  return isnumeric (id) && atoi (id) ? rhtunit_find_at (atoi (id) - 1) : rhtunit_find_by_name (id);
}


/* Longest name */
unsigned rhtunit_all_maxn (void)
{
  unsigned n = 0;
  unsigned i;
  for (i = 0; i < RHTUNIT_NO; i ++)
    n = RMAX (n, strlen (builtins [i] . name));
  return n;
}


/* Longest name */
unsigned rhtunit_maxn (rhtunit_t * argv [])
{
  unsigned n = 0;
  while (argv && * argv)
    {
      n = RMAX (n, strlen ((* argv) -> name));
      argv ++;
    }
  return n;
}


/* Run the Unit Tests included in argv[] */
void rhtunit_run (rhtunit_t * argv [], unsigned items)
{
  unsigned maxn = rhtunit_maxn (argv);
  unsigned n    = digits (arrlen (argv));
  unsigned seq  = 0;

  while (argv && * argv)
    rhtunit_run_one (* argv ++, items, n, ++ seq, maxn);
}


void rhtunit_print_no (void)
{
  printf ("Built-in Units Tests : %u\n", rhtunit_no ());
}


/* Print the Unit Tests included in argv[] */
void rhtunit_print (rhtunit_t * argv [])
{
  unsigned maxn = rhtunit_maxn (argv);
  unsigned i = 0;

  rhtunit_print_header (maxn);
  while (argv && * argv)
    rhtunit_print_one (* argv ++, ++ i, maxn);
}


/* Print the all the builtin Unit Tests */
void rhtunit_print_all (void)
{
  unsigned maxn = rhtunit_all_maxn ();
  unsigned i;

  rhtunit_print_header (maxn);
  for (i = 0; i <= rhtunit_no (); i ++)
    rhtunit_print_one (rhtunit_find_at (i), i + 1, maxn);
}
