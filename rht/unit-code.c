/* System headers */
#include <assert.h>

/* Project headers */
typedef struct rht rht_t;
#include "rht.h"
#include "vargs.h"
#include "datasets.h"


/* === Implementation of the built-in Unit Tests === */


/* Callback to iterate over the hash table */
static void addone (void * x)
{
  (* (unsigned *) x) ++;
}


/* Allocate and populate a hash table with n objects */
static rht_t * populate (unsigned argc, robj_t * argv [])
{
  rht_t * ht = rht_alloc (argc);
  unsigned i;
  assert (ht);
  assert (rht_count (ht) == 0);
  for (i = 0; i < argc; i ++)
    {
      rht_set (ht, argv [i] -> skey, argv [i]);
      assert (rht_count (ht) == i + 1);
    }
  assert (rht_count (ht) == argc);
  return ht;
}


unsigned alloc_free (unsigned argc)
{
  rht_t * ht = rht_alloc (argc);
  assert (ht);
  assert (rht_count (ht) == 0);
  rht_free (ht);
  return argc;
}


unsigned alloc_add_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  rht_free (ht);
  rmobjs (argv);
  return argc;
}


unsigned alloc_add_clear_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  rht_clear (ht);
  assert (rht_count (ht) == 0);
  rht_free (ht);
  rmobjs (argv);
  return argc;
}


unsigned alloc_add_count_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  unsigned count = rht_count (ht);
  assert (count == argc);
  rht_free (ht);
  rmobjs (argv);
  return count;
}


unsigned alloc_add_found_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    {
      robj_t * found = rht_get (ht, argv [i] -> skey);
      assert (found);
      assert (found -> ukey == argv [i] -> ukey);          /* Dereference */
    }
  rht_free (ht);
  rmobjs (argv);
  return argc;
}


unsigned alloc_add_notfound_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    assert (! rht_get (ht, argv [i] -> smiss));
  rht_free (ht);
  rmobjs (argv);
  return argc;
}


unsigned alloc_add_delete_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    {
      rht_del (ht, argv [i] -> skey);
      assert (rht_count (ht) == argc - i - 1);
    }
  assert (rht_count (ht) == 0);
  rht_free (ht);
  rmobjs (argv);
  return argc;
}


/* Attempt to delete a non-existing */
unsigned alloc_add_missed_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    {
      rht_del (ht, argv [i] -> smiss);
      assert (rht_count (ht) == argc);
    }
  assert (rht_count (ht) == argc);
  rht_free (ht);
  rmobjs (argv);
  return argc;
}


unsigned alloc_add_iterate_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  unsigned count = 0;
  rht_foreach (ht, addone, & count);
  assert (rht_count (ht) == count);
  rht_free (ht);
  rmobjs (argv);
  return argc;
}


unsigned alloc_add_keys_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  char ** keys = rht_keys (ht);
  char ** k = keys;
  assert (valen ((void **) keys) == argc);
  while (k && * k)
    assert (rht_has (ht, * k ++));
  vaclear ((void **) keys, NULL);
  rht_free (ht);
  rmobjs (argv);
  return argc;
}


unsigned alloc_add_vals_free (unsigned argc)
{
  robj_t ** argv = mkobjs (argc);
  rht_t * ht = populate (argc, argv);
  void ** vals = rht_vals (ht);
  assert (valen (vals) == argc);
  vaclear (vals, NULL);
  rht_free (ht);
  rmobjs (argv);
  return argc;
}
