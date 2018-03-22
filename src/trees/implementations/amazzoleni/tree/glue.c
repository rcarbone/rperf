/* System headers */
#include <stdlib.h>


/* The implementation */
#include "tommytree.c"


/* Project headers */

/* librtree - an abstract C library over real tree implementations */
typedef tommy_tree rtree_t;
#include "rtree-api.h"

/* Node definition */
#include "nodes.h"
#include "safe.h"


static int nodecmp (const void * a, const void * b)
{
  return ((rnode_t *) a) -> val - ((rnode_t *) b) -> val;
}


rtree_t * rtree_alloc (void)
{
  rtree_t * tree = calloc (1, sizeof (* tree));
  tommy_tree_init (tree, nodecmp);
  return tree;
}


void rtree_free (rtree_t * tree)
{
  safefree (tree);
}


void rtree_foreach (rtree_t * tree, rtree_each_f * fn, void * data)
{
  unsigned i = tommy_tree_count (tree);
  while (i --)
    if (fn)
      fn (data);
}


unsigned rtree_count (rtree_t * tree)
{
  return tommy_tree_count (tree);
}


void rtree_add (rtree_t * tree, void * node)
{
  tommy_tree_insert (tree, & ((rnode_t *) node) -> tommy, ((rnode_t *) node) -> key);
}


void * rtree_get (rtree_t * tree, void * node)
{
  return tommy_tree_search (tree, ((rnode_t *) node) -> key);
}


void rtree_del (rtree_t * tree, void * node)
{
  tommy_tree_remove (tree, ((rnode_t *) node) -> key);
}
