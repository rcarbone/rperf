/*
 * All the Unit Tests are here grouped together in a static table.
 *
 * Each implementation of a single Unit test operates with 'n' different unique keys and
 * all are based on the assert(3) to check for assertions.
 */


/* System headers */
#include <assert.h>

/* Project headers */
typedef struct rhash rhash_t;
#include "rhash.h"

#include "varrays.h"
#include "datasets.h"
#include "rwall.h"
#include "rctype.h"
#include "rtest.h"


/* The local identifiers for the Unit Tests to run (indexed by keys) */
typedef enum
{
  RUNIT_MAKE    = 0x01,  /* Allocate and free an empty container                                  */
  RUNIT_FILL    = 0x02,  /* Allocate, populate and free a container                               */
  RUNIT_CLEAR   = 0x03,  /* Allocate, populate, clear and free a container                        */
  RUNIT_COUNT   = 0x04,  /* Allocate, populate, count and free a container                        */
  RUNIT_FOUND   = 0x05,  /* Allocate, populate, find all existing elements one-by-one and free    */
  RUNIT_MISS    = 0x06,  /* Allocate, populate, find non existing elements and free               */
  RUNIT_DELETE  = 0x07,  /* Allocate, populate, delete all existing elements one-by-one and free  */
  RUNIT_MISSED  = 0x08,  /* Allocate, populate, delete non existing elements and free             */
  RUNIT_FOREACH = 0x09,  /* Allocate, populate, iterate all existing elements one-by-one and free */
  RUNIT_KEYS    = 0x0a,  /* Allocate, populate, iterate to get all keys one-by-one and free       */
  RUNIT_VALS    = 0x0b,  /* Allocate, populate, iterate to get all values one-by-one and free     */

} runit_id_t;


/* Forward */
static runit_f alloc_free;
static runit_f alloc_add_free;
static runit_f alloc_add_clear_free;
static runit_f alloc_add_count_free;
static runit_f alloc_add_found_free;
static runit_f alloc_add_notfound_free;
static runit_f alloc_add_delete_free;
static runit_f alloc_add_missed_free;
static runit_f alloc_add_iterate_free;
static runit_f alloc_add_keys_free;
static runit_f alloc_add_vals_free;


/* All the Unit Tests in an array */
static rtest_t runit_builtins [] =
{
  { RUNIT_MAKE,    "make",    "Allocate and free an empty container", alloc_free,              NULL },
  { RUNIT_FILL,    "fill",    "Add objects to an empty container",    alloc_add_free,          NULL },
  { RUNIT_CLEAR,   "clear",   "Add objects and clear",                alloc_add_clear_free,    NULL },
  { RUNIT_COUNT,   "count",   "Add objects and count",                alloc_add_count_free,    NULL },
  { RUNIT_FOUND,   "found",   "Find existing objects",                alloc_add_found_free,    NULL },
  { RUNIT_MISS,    "miss",    "Search non existing objects",          alloc_add_notfound_free, NULL },
  { RUNIT_DELETE,  "delete",  "Delete existing objects",              alloc_add_delete_free,   NULL },
  { RUNIT_MISSED,  "missed",  "Delete non existing objects",          alloc_add_missed_free,   NULL },
  { RUNIT_FOREACH, "foreach", "Iterate existing objects",             alloc_add_iterate_free,  NULL },
  { RUNIT_KEYS,    "keys",    "Get all the keys",                     alloc_add_keys_free,     NULL },
  { RUNIT_VALS,    "vals",    "Get all the values",                   alloc_add_vals_free,     NULL },
};
#define RUNIT_NO (sizeof (runit_builtins) / sizeof (* runit_builtins))


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* === Implementation of Unit Tests === */

/* Callback to iterate over the hash table */
static void addone (void * x)
{
  (* (unsigned *) x) ++;
}


/* Allocate and populate a hash table with n objects */
static rhash_t * populate (unsigned argc, robj_t * argv [])
{
  rhash_t * ht = rhash_alloc (argc);
  unsigned i;
  assert (ht);
  assert (rhash_count (ht) == 0);
  for (i = 0; i < argc; i ++)
    {
      rhash_set (ht, argv [i] -> skey, argv [i]);
      assert (rhash_count (ht) == i + 1);
    }
  assert (rhash_count (ht) == argc);
  return ht;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static unsigned alloc_free (unsigned argc)
{
  rhash_t * ht = rhash_alloc (argc);
  assert (ht);
  assert (rhash_count (ht) == 0);
  rhash_free (ht);
  return argc;
}


static unsigned alloc_add_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  rhash_free (ht);
  rmobjs (argv);
  return argc;
}


static unsigned alloc_add_clear_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  rhash_clear (ht);
  assert (rhash_count (ht) == 0);
  rhash_free (ht);
  rmobjs (argv);
  return argc;
}


static unsigned alloc_add_count_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  unsigned count = rhash_count (ht);
  assert (count == argc);
  rhash_free (ht);
  rmobjs (argv);
  return count;
}


static unsigned alloc_add_found_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    {
      robj_t * found = rhash_get (ht, argv [i] -> skey);
      assert (found);
      assert (found -> ukey == argv [i] -> ukey);          /* Dereference */
    }
  rhash_free (ht);
  rmobjs (argv);
  return argc;
}


static unsigned alloc_add_notfound_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    assert (! rhash_get (ht, argv [i] -> smiss));
  rhash_free (ht);
  rmobjs (argv);
  return argc;
}


static unsigned alloc_add_delete_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    {
      rhash_del (ht, argv [i] -> skey);
      assert (rhash_count (ht) == argc - i - 1);
    }
  assert (rhash_count (ht) == 0);
  rhash_free (ht);
  rmobjs (argv);
  return argc;
}


/* Attempt to delete a non-existing */
static unsigned alloc_add_missed_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    {
      rhash_del (ht, argv [i] -> smiss);
      assert (rhash_count (ht) == argc);
    }
  assert (rhash_count (ht) == argc);
  rhash_free (ht);
  rmobjs (argv);
  return argc;
}


static unsigned alloc_add_iterate_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  unsigned count = 0;
  rhash_foreach (ht, addone, & count);
  assert (rhash_count (ht) == count);
  rhash_free (ht);
  rmobjs (argv);
  return argc;
}


static unsigned alloc_add_keys_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  char ** keys = rhash_keys (ht);
  char ** k = keys;
  assert (valen ((void **) keys) == argc);
  while (k && * k)
    assert (rhash_has (ht, * k ++));
  vaclear ((void **) keys, NULL);
  rhash_free (ht);
  rmobjs (argv);
  return argc;
}


static unsigned alloc_add_vals_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rhash_t * ht = populate (argc, argv);
  void ** vals = rhash_vals (ht);
  assert (valen (vals) == argc);
  vaclear (vals, NULL);
  rhash_free (ht);
  rmobjs (argv);
  return argc;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static void runit_run_one (rtest_t * runit, unsigned items)
{
  if (runit)
    {
      unsigned n;
      printf ("Running %s ... ", runit -> name);
      n = runit -> unit (items);
      if (n == items)
	printf ("Ok\n");
      else
	printf ("No\n");
    }
}


/* Return the # of Unit Tests */
unsigned runit_no (void)
{
  return RUNIT_NO;
}


/* Return the handle of the first Unit Test */
rtest_t * runit_first (void)
{
  return & runit_builtins [0];
}


/* Return the handle of the last Unit Test */
rtest_t * runit_last (void)
{
  return & runit_builtins [RUNIT_NO - 1];
}


/* Return the handle of the next Unit Test */
rtest_t * runit_next (void)
{
  static unsigned i = 0;
  return & runit_builtins [i ++ % RUNIT_NO];
}


/* Return the handle of the next Unit Test randomly generated */
rtest_t * runit_next_rnd (void)
{
  return & runit_builtins [rrand (RUNIT_NO)];
}


/* Looukup for an Unit Test by index */
rtest_t * runit_find_at (unsigned i)
{
  return i < RUNIT_NO ? & runit_builtins [i] : NULL;
}


/* Looukup for an Unit Test by its unique id */
rtest_t * runit_find_by_id (unsigned id)
{
  unsigned i;
  for (i = 0; i < RUNIT_NO; i ++)
    if (runit_builtins [i] . id == id)
      return & runit_builtins [i];
  return NULL;
}


/* Looukup for an Unit Test by its unique name */
rtest_t * runit_find_by_name (char * name)
{
  unsigned i;
  for (i = 0; name && i < RUNIT_NO; i ++)
    if (! strcmp (runit_builtins [i] . name, name))
      return & runit_builtins [i];
  return NULL;
}


/* Return all the Unit Tests in an array in the same order they were defined */
rtest_t ** runit_all (void)
{
  unsigned i;
  rtest_t ** all = NULL;
  for (i = 0; i < RUNIT_NO; i ++)
    all = arrmore (all, & runit_builtins [i], rtest_t);
  return all;
}


/* Return all the Unit Tests in an array in the same order they were defined starting at given offset */
rtest_t ** runit_all_n (unsigned n)
{
  rtest_t ** all;
  unsigned i;

  n %= RUNIT_NO;

  for (i = 0; i < n; i ++)
    runit_next ();

  all = arrmore (NULL, runit_next (), rtest_t);
  while (arrlen (all) != runit_no ())
    all = arrmore (all, runit_next (), rtest_t);
  return all;
}


rtest_t ** runit_all_rnd (void)
{
  return (rtest_t **) varnd (runit_no (), (void **) runit_all ());
}


rtest_t * runit_valid (char * id)
{
  return isnumeric (id) && atoi (id) ? runit_find_at (atoi (id) - 1) : runit_find_by_name (id);
}


/* Run the Unit Tests included in argv[] */
void runit_run_these (rtest_t * argv [], unsigned items)
{
  while (argv && * argv)
    runit_run_one (* argv ++, items);
}
