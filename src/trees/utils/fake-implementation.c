/* System headers */
#include <stdlib.h>


/* Project headers */
#include "vargv.h"
#include "nodes.h"
#include "safe.h"


/* This implementation uses a dynamic array of void pointers just as example */
typedef struct
{
  rnode_t ** tree;

} rtree_t;


/* librtree - an abstract C library over real tree implementations */
#include "rtree-api.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rtree_t * rtree_alloc (void)
{
  return (rtree_t *) calloc (1, sizeof (rtree_t));
}


void rtree_free (rtree_t * tree)
{
  arrclear (tree -> tree, NULL);
  safefree (tree);
}


void rtree_foreach (rtree_t * tree, rtree_each_f * fn, void * data)
{
  rnode_t ** node = tree -> tree;
  while (node && * node ++)
    if (fn)
      fn (data);
}


unsigned rtree_count (rtree_t * tree)
{
  return (arrlen (tree -> tree));
}


void rtree_add (rtree_t * tree, void * node)
{
  tree -> tree = arrmore (tree -> tree, node, rnode_t);
}


void * rtree_get (rtree_t * tree, void * node)
{
  rnode_t ** n = tree -> tree;
  while (n && * n)
    if (* n == node)
      return node;
    else
      n ++;
  return NULL;
}


void rtree_del (rtree_t * tree, void * node)
{
  if (rtree_get (tree, node))
    tree -> tree = arrless (tree -> tree, node, rnode_t, NULL);
}
