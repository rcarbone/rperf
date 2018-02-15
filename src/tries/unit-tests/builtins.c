/*
 * All the Unit Tests are here, grouped together in a static table.
 */


/* System headers */
#include <assert.h>

/* Project headers */
#define NEED_RTRIE_TYPEDEF
#include "rtrie-api.h"
#include "rut.h"
#include "vargv.h"
#include "nodes.h"


/* Forward declarations */
static rut_f alloc_free;
static rut_f alloc_add_free;
static rut_f alloc_iterate_free;
static rut_f alloc_count_free;
static rut_f alloc_found_free;
static rut_f alloc_miss_free;
static rut_f alloc_delete_free;
static rut_f alloc_missed_free;


/* All the Unit Tests in a static table */
static rut_t builtins [] =
{
  { "make",    "Allocate and free an empty container", alloc_free         },
  { "add",     "Add elements one-by-one",              alloc_add_free     },
  { "iterate", "Iterate over existent one-by-one",     alloc_iterate_free },
  { "count",   "Add elements and count",               alloc_count_free   },
  { "found",   "Search for existent one-by-one",       alloc_found_free   },
  { "miss",    "Search for non-existent one-by-one",   alloc_miss_free    },
  { "delete",  "Delete existent one-by-one",           alloc_delete_free  },
  { "missed",  "Delete non-existent one-by-one",       alloc_missed_free  },
};
#define RUTNO (sizeof (builtins) / sizeof (* builtins))


/* === Implementation of the builtin Unit Tests === */


/* Callback to iterate over the elements */
static void addone_cb (void * x)
{
  (* (unsigned *) x) ++;
}


/* Allocate and populate a trie inserting argc elements */
static rtrie_t * populate (unsigned argc, rnode_t * argv [])
{
  rtrie_t * trie = rtrie_alloc ();
  unsigned i;
  assert (trie);
  assert (rtrie_count (trie) == 0);
  for (i = 0; i < argc; i ++)
    rtrie_add (trie, argv [i]);
  assert (rtrie_count (trie) == argc);
  return trie;
}


/* Unit test - memory */
static unsigned alloc_free (unsigned argc)
{
  rtrie_t * trie = populate (0, NULL);
  assert (trie);
  assert (rtrie_count (trie) == 0);
  rtrie_free (trie);
  return argc;
}


/* Unit test - insertions */
static unsigned alloc_add_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtrie_t * trie = populate (argc, argv);
  rtrie_free (trie);
  rmnodes (argv);
  return argc;
}


/* Unit test - iterating */
static unsigned alloc_iterate_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtrie_t * trie = populate (argc, argv);
  unsigned count = 0;
  rtrie_foreach (trie, addone_cb, & count);
  assert (rtrie_count (trie) == count);
  rtrie_free (trie);
  rmnodes (argv);
  return argc;
}


/* Unit test - counting */
static unsigned alloc_count_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtrie_t * trie = populate (argc, argv);
  unsigned count = rtrie_count (trie);
  rtrie_free (trie);
  rmnodes (argv);
  return count;
}


/* Unit test - getting existing values (successful lookup) */
static unsigned alloc_found_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtrie_t * trie = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    assert (rtrie_get (trie, argv [i]));
  rtrie_free (trie);
  rmnodes (argv);
  return argc;
}


/* Unit test - getting nonexistent values (lookup failure) */
static unsigned alloc_miss_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rnode_t ** miss = mkmiss (argc);
  rtrie_t * trie = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    assert (! rtrie_get (trie, miss [i]));
  rtrie_free (trie);
  rmnodes (miss);
  rmnodes (argv);
  return argc;
}


/* Unit test - deleting existing values (successful lookup) */
static unsigned alloc_delete_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rtrie_t * trie = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    rtrie_del (trie, argv [i]);
  assert (rtrie_count (trie) == 0);
  rtrie_free (trie);
  rmnodes (argv);
  return argc;
}


/* Unit test - deleting nonexistent values (lookup failure) */
static unsigned alloc_missed_free (unsigned argc)
{
  rnode_t ** argv = mknodes (argc);
  rnode_t ** miss = mkmiss (argc);
  rtrie_t * trie = populate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    {
      rtrie_del (trie, miss [i]);
      assert (rtrie_count (trie) == argc);
    }
  assert (rtrie_count (trie) == argc);
  rtrie_free (trie);
  rmnodes (miss);
  rmnodes (argv);
  return argc;
}


/* Return the # of Unit Tests */
unsigned rtrie_ut_argc (void)
{
  return RUTNO;
}


/* Return the table of the Unit Tests */
rut_t * rtrie_ut_argv (void)
{
  return builtins;
}
