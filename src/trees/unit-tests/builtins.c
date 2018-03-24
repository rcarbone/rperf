/*
 * All the Unit Tests are here, grouped together in a static table.
 */


/* System headers */
#include <assert.h>

/* Project headers */
#define NEED_RTREE_TYPEDEF
#include "rtree-api.h"
#include "rut.h"
#include "vargv.h"
#include "rnodes.h"

#ifdef __cplusplus
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#endif
#endif /* __cplusplus */


/* Forward declarations */
static rut_f alloc_free;
static rut_f alloc_add_free;
static rut_f alloc_count_free;
static rut_f alloc_found_free;
static rut_f alloc_miss_free;
static rut_f alloc_delete_free;
static rut_f alloc_missed_free;
static rut_f alloc_iterate_free;


/* All the Unit Tests in a static table */
static rut_t builtins [] =
{
  { "make",    "Allocate and free an empty container", alloc_free         },
  { "add",     "Add elements one-by-one",              alloc_add_free     },
  { "count",   "Add elements and count",               alloc_count_free   },
  { "found",   "Search for existent one-by-one",       alloc_found_free   },
  { "miss",    "Search for non-existent one-by-one",   alloc_miss_free    },
  { "delete",  "Delete existent one-by-one",           alloc_delete_free  },
  { "missed",  "Delete non-existent one-by-one",       alloc_missed_free  },
  { "iterate", "Iterate over existent one-by-one",     alloc_iterate_free },
};
#define RUTNO (sizeof (builtins) / sizeof (* builtins))


/* === Implementation of the builtin Unit Tests === */


/* Callback to iterate over the elements */
static void addone_cb (void * x)
{
  (* (unsigned *) x) ++;
}


/* Allocate and populate a tree inserting argc elements */
static rtree_t * populate (unsigned argc, rnode_t * argv [])
{
  rtree_t * tree = rtree_alloc ();
  unsigned i;
  assert (tree);
  assert (rtree_count (tree) == 0);
  for (i = 0; i < argc; i ++)
    rtree_add (tree, argv [i]);
  assert (rtree_count (tree) == argc);
  return tree;
}


/* Unit test - memory */
static unsigned alloc_free (unsigned argc)
{
  rtree_t * tree = populate (0, NULL);
  assert (tree);
  assert (rtree_count (tree) == 0);
  rtree_free (tree);
  return argc;
}


/* Unit test - insertions */
static unsigned alloc_add_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtree_t * tree = populate (argc, argv);
  rtree_free (tree);
  rmnodes (argv);
  return argc;
}


/* Unit test - iterating */
static unsigned alloc_iterate_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtree_t * tree = populate (argc, argv);
  unsigned count = 0;
  rtree_foreach (tree, addone_cb, & count);
  assert (rtree_count (tree) == count);
  rtree_free (tree);
  rmnodes (argv);
  return argc;
}


/* Unit test - counting */
static unsigned alloc_count_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtree_t * tree = populate (argc, argv);
  unsigned count = rtree_count (tree);
  rtree_free (tree);
  rmnodes (argv);
  return count;
}


/* Unit test - getting existing values (successful lookup) */
static unsigned alloc_found_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtree_t * tree = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    assert (rtree_get (tree, argv [i]));
  rtree_free (tree);
  rmnodes (argv);
  return argc;
}


/* Unit test - getting nonexistent values (lookup failure) */
static unsigned alloc_miss_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rnode_t ** miss = mkmiss (argc);
  rtree_t * tree = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    assert (! rtree_get (tree, miss [i]));
  rtree_free (tree);
  rmnodes (miss);
  rmnodes (argv);
  return argc;
}


/* Unit test - deleting existing values (successful lookup) */
static unsigned alloc_delete_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtree_t * tree = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    rtree_del (tree, argv [i]);
  assert (rtree_count (tree) == 0);
  rtree_free (tree);
  rmnodes (argv);
  return argc;
}


/* Unit test - deleting nonexistent values (lookup failure) */
static unsigned alloc_missed_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rnode_t ** miss = mkmiss (argc);
  rtree_t * tree = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    {
      rtree_del (tree, miss [i]);
      assert (rtree_count (tree) == argc);
    }
  assert (rtree_count (tree) == argc);
  rtree_free (tree);
  rmnodes (miss);
  rmnodes (argv);
  return argc;
}


/* Return the # of Unit Tests */
unsigned rtree_ut_argc (void)
{
  return RUTNO;
}


/* Return the table of the Unit Tests */
rut_t * rtree_ut_argv (void)
{
  return builtins;
}
