/* System headers */
#include <stdlib.h>


/* The implementation */
#include "skiplist.c"
#include "tree_common.c"
#include "dict.c"


/* Project headers */

/* librtree - an abstract C library over real tree implementations */
typedef skiplist rtree_t;
#include "rtree-api.h"

/* Node definition */
#include "rnodes.h"


rtree_t * rtree_alloc (void)
{
  return skiplist_new (cmpnode, MAX_LINK);
}


void rtree_free (rtree_t * tree)
{
  skiplist_free (tree, NULL);
}


void rtree_add (rtree_t * tree, void * node)
{
  skiplist_insert (tree, node);
}


void * rtree_get (rtree_t * tree, void * node)
{
  return skiplist_search (tree, node) ? node : NULL;
}


void rtree_del (rtree_t * tree, void * node)
{
  skiplist_remove (tree, node);
}


unsigned rtree_count (rtree_t * tree)
{
  return skiplist_count (tree);
}


void rtree_foreach (rtree_t * tree, rtree_each_f * fn, void * data)
{
  unsigned i = skiplist_count (tree);
  while (i --)
    if (fn)
      fn (data);
}
