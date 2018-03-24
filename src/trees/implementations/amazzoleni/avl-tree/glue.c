/* System headers */
#include <stdlib.h>


/* The implementation */
#include "tommytree.c"


/* Project headers */

/* librtree - an abstract C library over real tree implementations */
typedef tommy_tree rtree_t;
#include "rtree-api.h"

/* Node definition */
#include "rnodes.h"


/* Typedef used in the foreach callback */
typedef struct
{
  rtree_each_f * fn;
  void * data;

} func_t;


/* Callback to iterate over the hash table to search for a key */
static void foreach_cb (void * arg, void * obj)
{
  func_t * func = arg;
  func -> fn (func -> data);
}


rtree_t * rtree_alloc (void)
{
  rtree_t * tree = calloc (1, sizeof (* tree));
  tommy_tree_init (tree, cmpnode);
  return tree;
}


void rtree_free (rtree_t * tree)
{
  free (tree);
}


void rtree_add (rtree_t * tree, void * node)
{
  tommy_tree_insert (tree, & ((rnode_t *) node) -> tommy, node);
}


void * rtree_get (rtree_t * tree, void * node)
{
  return tommy_tree_search (tree, node);
}


void rtree_del (rtree_t * tree, void * node)
{
  tommy_tree_remove (tree, node);
}


unsigned rtree_count (rtree_t * tree)
{
  return tommy_tree_count (tree);
}


void rtree_foreach (rtree_t * tree, rtree_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  tommy_tree_foreach_arg (tree, foreach_cb, & fun);
}
