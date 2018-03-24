/* System headers */
#include <stdlib.h>
#include <string.h>


/* The implementation */
#include "tree.h"

/* Project headers */

/* librtree - an abstract C library over real tree implementations */
#define NEED_RTREE_TYPEDEF
#include "rtree-api.h"

/* Node definition */
#include "rnodes.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* Forward */
static int nodecmp_2 (rnode_t * a, rnode_t * b);

/* Tree naming and definition */
SPLAY_HEAD(rtree, rnode);

/* Generates prototypes and inline functions */
SPLAY_PROTOTYPE (rtree, rnode, _lest_, nodecmp_2)
SPLAY_GENERATE(rtree, rnode, _lest_, nodecmp_2);


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/* Compare functions */
static int _nodecmp_ (rnode_t * a, rnode_t * b) { return (a -> val > b -> val) - (a -> val < b -> val); }


/* Duplicates are not allowed in the tree, so force an arbitrary ordering for non-identical items with equal keys */
static int nodecmp_2 (rnode_t * a, rnode_t * b)
{
  int ret = _nodecmp_ (a, b);
  if (ret == 0)
    ret = ((uintptr_t) a > (uintptr_t) b) - ((uintptr_t) a < (uintptr_t) b);
  return ret;
}


static unsigned SPLAY_SIZE (rtree_t * tree)
{
  unsigned size = 0;
  rnode_t * node;
  SPLAY_FOREACH (node, rtree, tree)
    size ++;
  return size;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rtree_t * rtree_alloc (void)
{
  rtree_t * tree = calloc (1, sizeof (* tree));
  SPLAY_INIT (tree);
  return tree;
}


void rtree_free (rtree_t * tree)
{
  free (tree);
}


void rtree_clear (rtree_t * tree)
{
#if defined(ROCCO)
  SPLAY_CLEAR (rtree, tree);
#endif /* ROCCO */
}


unsigned rtree_count (rtree_t * tree)
{
  return SPLAY_SIZE (tree);
}


void rtree_add (rtree_t * tree, void * node)
{
  SPLAY_INSERT (rtree, tree, node);
}


void * rtree_get (rtree_t * tree, void * node)
{
  rnode_t * hit = SPLAY_FIND (rtree, tree, node);
  return hit ? hit -> foo : NULL;
}


void rtree_del (rtree_t * tree, void * node)
{
  rnode_t * hit = SPLAY_FIND (rtree, tree, node);
  if (hit)
    SPLAY_REMOVE (rtree, tree, hit);
}


bool rtree_has (rtree_t * tree, char * key)
{
  return rtree_get (tree, key);
}


void rtree_foreach (rtree_t * tree, rtree_each_f * fn, void * data)
{
  rnode_t * node;
  SPLAY_FOREACH (node, rtree, tree)
    fn (data);
}


char ** rtree_keys (rtree_t * tree)
{
  char ** keys = calloc (rtree_count (tree) + 1, sizeof (char *));
  unsigned i = 0;
  rnode_t * k;
  SPLAY_FOREACH (k, rtree, tree)
    keys [i ++] = k -> key;
  return keys;
}


void ** rtree_vals (rtree_t * tree)
{
  void ** vals = calloc (rtree_count (tree) + 1, sizeof (void *));
  unsigned i = 0;
  rnode_t * k;
  SPLAY_FOREACH (k, rtree, tree)
    vals [i ++] = k -> foo;
  return vals;
}
