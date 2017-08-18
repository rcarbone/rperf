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
#include "elems.h"
#include "rltest.h"


/* The local identifiers for the Unit Tests to run */
typedef enum
{
  RLUNIT_MAKE    = 0x01,
  RLUNIT_PREPEND = 0x02,
  RLUNIT_APPEND  = 0x03,
  RLUNIT_COUNT   = 0x04,
  RLUNIT_CLEAR   = 0x05,
  RLUNIT_FOUND   = 0x06,
  RLUNIT_MISS    = 0x07,
  RLUNIT_DELETE  = 0x08,
  RLUNIT_MISSED  = 0x09,
  RLUNIT_FOREACH = 0x0a,

} rlunit_id_t;


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


/* All the Unit Tests in an array */
static rltest_t rlunit_builtins [] =
{
  { RLUNIT_MAKE,    "make",    "Allocate and free an empty container", alloc_free         },
  { RLUNIT_PREPEND, "prepend", "Add elements at the head of the list", alloc_prepend_free },
  { RLUNIT_APPEND,  "append",  "Add elements at the tail of the list", alloc_append_free  },
  { RLUNIT_CLEAR,   "clear",   "Add elements and clear",               alloc_clear_free   },
  { RLUNIT_COUNT,   "count",   "Add elements and count",               alloc_count_free   },
  { RLUNIT_FOUND,   "found",   "Search for existent one-by-one",       alloc_found_free   },
  { RLUNIT_MISS,    "miss",    "Search for non-existent one-by-one",   alloc_miss_free    },
  { RLUNIT_DELETE,  "delete",  "Delete existent one-by-one",           alloc_delete_free  },
  { RLUNIT_MISSED,  "missed",  "Delete non-existent one-by-one",       alloc_missed_free  },
  { RLUNIT_FOREACH, "foreach", "Iterate over existent one-by-one",     alloc_iterate_free },
};
#define RLUNIT_NO (sizeof (rlunit_builtins) / sizeof (* rlunit_builtins))


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


static void rlunit_run_one (rltest_t * rlunit, unsigned items, unsigned n, unsigned seq, unsigned maxn)
{
  unsigned t;

  print_dots (rlunit -> name, "Running", n, seq, maxn);

  t = rlunit -> unit (items);
  if (t == items)
    printf ("Ok\n");
  else
    printf ("No\n");
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Return the # of Unit Tests */
unsigned rlunit_no (void)
{
  return RLUNIT_NO;
}


/* Return the handle of the first Unit Test */
rltest_t * rlunit_first (void)
{
  return & rlunit_builtins [0];
}


/* Return the handle of the last Unit Test */
rltest_t * rlunit_last (void)
{
  return & rlunit_builtins [RLUNIT_NO - 1];
}


/* Return the handle of the next Unit Test */
rltest_t * rlunit_next (void)
{
  static unsigned i = 0;
  return & rlunit_builtins [i ++ % RLUNIT_NO];
}


/* Return the handle of the next Unit Test randomly generated */
rltest_t * rlunit_next_rnd (void)
{
  return & rlunit_builtins [rrand (RLUNIT_NO)];
}


/* Looukup for an Unit Test by index */
rltest_t * rlunit_find_at (unsigned i)
{
  return i < RLUNIT_NO ? & rlunit_builtins [i] : NULL;
}


/* Looukup for an Unit Test by its unique id */
rltest_t * rlunit_find_by_id (unsigned id)
{
  unsigned i;
  for (i = 0; i < RLUNIT_NO; i ++)
    if (rlunit_builtins [i] . id == id)
      return & rlunit_builtins [i];
  return NULL;
}


/* Looukup for an Unit Test by its unique name */
rltest_t * rlunit_find_by_name (char * name)
{
  unsigned i;
  if (name)
    for (i = 0; i < RLUNIT_NO; i ++)
      if (! strcmp (rlunit_builtins [i] . name, name))
	return & rlunit_builtins [i];
  return NULL;
}


/* Return all the Unit Test names in an array in the same order they were defined */
char ** rlunit_names (void)
{
  unsigned i;
  char ** all = NULL;
  for (i = 0; i < RLUNIT_NO; i ++)
    all = argsmore (all, rlunit_builtins [i] . name);
  return all;
}


/* Return all the Unit Tests handles in an array in the same order they were defined */
rltest_t ** rlunit_all (void)
{
  unsigned i;
  rltest_t ** all = NULL;
  for (i = 0; i < RLUNIT_NO; i ++)
    all = arrmore (all, & rlunit_builtins [i], rltest_t);
  return all;
}


/* Return all the Unit Tests in an array in the same order they were defined starting at given offset */
rltest_t ** rlunit_all_n (unsigned n)
{
  rltest_t ** all;
  unsigned i;

  n %= RLUNIT_NO;

  for (i = 0; i < n; i ++)
    rlunit_next ();

  all = arrmore (NULL, rlunit_next (), rltest_t);
  while (arrlen (all) != rlunit_no ())
    all = arrmore (all, rlunit_next (), rltest_t);
  return all;
}


rltest_t ** rlunit_all_rnd (void)
{
  return (rltest_t **) varnd (rlunit_no (), (void **) rlunit_all ());
}


/* Longest name */
unsigned rlunit_all_maxn (void)
{
  unsigned n = 0;
  unsigned i;
  for (i = 0; i < RLUNIT_NO; i ++)
    n = RMAX (n, strlen (rlunit_builtins [i] . name));
  return n;
}


/* Longest name */
unsigned rlunit_maxn (rltest_t * argv [])
{
  unsigned n = 0;
  while (argv && * argv)
    {
      n = RMAX (n, strlen ((* argv) -> name));
      argv ++;
    }
  return n;
}


rltest_t * rlunit_valid (char * id)
{
  return isnumeric (id) && atoi (id) ? rlunit_find_at (atoi (id) - 1) : rlunit_find_by_name (id);
}


/* Run the Unit Tests included in argv[] */
void rlunit_run (rltest_t * argv [], unsigned items)
{
  unsigned maxn = rlunit_maxn (argv);
  unsigned n    = digits (arrlen (argv));
  unsigned seq  = 0;

  while (argv && * argv)
    rlunit_run_one (* argv ++, items, n, ++ seq, maxn);
}
