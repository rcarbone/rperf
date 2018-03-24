/* System headers */
#include <stdlib.h>


/* The implementation */
#include "hb_tree.c"
#include "tree_common.c"
#include "dict.c"


/* Project headers */

/* librtree - an abstract C library over real tree implementations */
typedef hb_tree rtree_t;
#include "rtree-api.h"

/* Node definition */
#include "rnodes.h"


rtree_t * rtree_alloc (void)
{
  return hb_tree_new (cmpnode);
}


void rtree_free (rtree_t * tree)
{
  hb_tree_free (tree, NULL);
}


void rtree_add (rtree_t * tree, void * node)
{
  hb_tree_insert (tree, node);
}


void * rtree_get (rtree_t * tree, void * node)
{
  return hb_tree_search (tree, node) ? node : NULL;
}


void rtree_del (rtree_t * tree, void * node)
{
  hb_tree_remove (tree, node);
}


unsigned rtree_count (rtree_t * tree)
{
  return hb_tree_count (tree);
}


void rtree_foreach (rtree_t * tree, rtree_each_f * fn, void * data)
{
  unsigned i = hb_tree_count (tree);
  while (i --)
    if (fn)
      fn (data);
}
