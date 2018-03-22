/*
 * The Suite for Trees implementations is here.
 *
 * All the functions are grouped together in a static table.
 */


/* Project headers */
#define NEED_RTREE_TYPEDEF
#include "rtree-api.h"
#include "rsuite.h"
#include "sargv.h"
#include "rwall.h"
#include "rctype.h"
#include "support.h"
#include "nodes.h"


/* The Suite in a static table */
static rsuite_t builtins [] =
{
  { "grow_seq", "Populate an empty container [unique sequential keys]", rsuite_grow_seq },
  { "grow_rnd", "Populate an empty container [unique random keys]",     rsuite_grow_rnd },
  { "hit_seq",  "Search one-by-one all existing [sequential keys]",     rsuite_hit_seq  },
  { "hit_rnd",  "Search one-by-one all existing [random keys]",         rsuite_hit_rnd  },

  { "pop",      "Remove all elements one-by-one", rsuite_pop     },

  { "iterate",  "Iterate over all tree elements", rsuite_iterate },
};
#define RSUITENO (sizeof (builtins) / sizeof (* builtins))


/* === Implementation of the builtin Suite === */


/* Callback to iterate over the tree elements */
static void addone_cb (void * x)
{
  (* (unsigned *) x) ++;
}


/* Allocate and populate a tree inserting argc elements */
static rtree_t * populate (unsigned argc, void * argv [])
{
  rtree_t * tree = rtree_alloc ();
  unsigned i;
  for (i = 0; i < argc; i ++)
    rtree_add (tree, argv [i]);
  return tree;
}


/* Allocate and populate a tree inserting argc elements in sequential order */
rtime_t rsuite_grow_seq (unsigned argc, void * argv [])
{
  rtree_t * tree = rtree_alloc ();
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rtree_add (tree, argv [i]);
  t2 = nswall ();
  i = rtree_count (tree);
  rtree_free (tree);
  return i == argc ? t2 - t1 : 0;
}


/* Allocate and populate a tree inserting argc elements in random order */
rtime_t rsuite_grow_rnd (unsigned argc, void * argv [])
{
  rtree_t * tree = rtree_alloc ();
  unsigned * order = rndorder (argc);
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rtree_add (tree, argv [order [i]]);
  t2 = nswall ();
  free (order);
  i = rtree_count (tree);
  rtree_free (tree);
  return i == argc ? t2 - t1 : 0;
}


/* Find and dereference with success all the items in sequential order */
rtime_t rsuite_hit_seq (unsigned argc, void * argv [])
{
  rtree_t * tree = populate (argc, argv);
  unsigned hit = 0;
  unsigned i;
  rtime_t t1;
  rtime_t t2;
  void * found;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    {
      found = rtree_get (tree, argv [i]);
      if (found && found == ((rnode_t *) argv [i]) -> foo)           /* dereference */
	hit ++;
    }
  t2 = nswall ();
  rtree_free (tree);
  return hit == argc ? t2 - t1 : 0;
}


/* Find and dereference with success all the items in random order */
rtime_t rsuite_hit_rnd (unsigned argc, void * argv [])
{
  rtree_t * tree = populate (argc, argv);
  unsigned * order = rndorder (argc);
  unsigned hit = 0;
  unsigned i;
  rtime_t t1;
  rtime_t t2;
  void * found;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    {
      found = rtree_get (tree, argv [order [i]]);
      if (found && found == ((rnode_t *) argv [order [i]]) -> foo)  /* dereference */
	hit ++;
    }
  t2 = nswall ();
  free (order);
  rtree_free (tree);
  return hit == argc ? t2 - t1 : 0;
}


/* Delete from the head of the tree all the elements one-by-one */
rtime_t rsuite_pop (unsigned argc, void * argv [])
{
  rtree_t * tree = populate (argc, argv);
  unsigned n = 0;
  unsigned i;
  rtime_t t1;
  rtime_t t2;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rtree_del (tree, argv [i]);
  t2 = nswall ();
  n = rtree_count (tree);
  rtree_free (tree);
  return n == 0 ? t2 - t1 : 0;
}


/* Iterate over all the tree elements */
rtime_t rsuite_iterate (unsigned argc, void * argv [])
{
  rtree_t * tree = populate (argc, argv);
  unsigned n = 0;
  rtime_t t1;
  rtime_t t2;
  t1 = nswall ();
  rtree_foreach (tree, addone_cb, & n);
  t2 = nswall ();
  rtree_free (tree);
  return n == argc ? t2 - t1 : 0;
}


/* Return the # of items */
unsigned rtree_suite_argc (void)
{
  return RSUITENO;
}


/* Return the table of the Suite */
rsuite_t * rtree_suite_argv (void)
{
  return builtins;
}
