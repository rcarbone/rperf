/* System headers */
#include <stdlib.h>
#include <string.h>


/* The implementation */
#include "ht-internal.h"

/* Project headers */

/* librtree - an abstract C library over real tree implementations */
#define NEED_RTREE_TYPEDEF
#include "rtree-api.h"

/* Node definition */
#include "rnodes.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


#define LOAD_LIMIT 0.9

/* Forward */
static unsigned hash_fn (rnode_t * node);
static int cmp_fn (rnode_t * a, rnode_t * b);

/* Hash table naming and definition */
HT_HEAD (rtree, rnode);

/* Generates prototypes and inline functions */
HT_PROTOTYPE (rtree, rnode, _levt_, hash_fn, cmp_fn)
HT_GENERATE (rtree, rnode, _levt_, hash_fn, cmp_fn, LOAD_LIMIT, malloc, realloc, free)

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
/* Basic string hash function from Python (as found in Niels Provos's hash table implementation) */
static unsigned rht_python_hash (const char * s)
{
  unsigned h;
  const unsigned char * cp = (const unsigned char *) s;
  h = * cp << 7;
  while (* cp)
    h = (1000003 * h) ^ * cp ++;

  /* This conversion truncates the length of the string, but that's ok */
  h ^= (unsigned) (cp - (const unsigned char *) s);

  return h;
}

/* Hash string keys using the internal helper function */
static unsigned hash_fn (rnode_t * node)
{
  return rht_python_hash (node -> key);
}


/* Compare 2 string keys for equality */
static int cmp_fn (rnode_t * n1, rnode_t * n2)
{
  return ! strcmp (n1 -> key, n2 -> key);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rtree_t * rtree_alloc (void)
{
  rtree_t * tree = calloc (1, sizeof (* tree));
  HT_INIT (rtree, tree);
  return tree;
}


void rtree_free (rtree_t * tree)
{
  HT_CLEAR (rtree, tree);
  free (tree);
}


void rtree_clear (rtree_t * tree)
{
  HT_CLEAR (rtree, tree);
}


unsigned rtree_count (rtree_t * tree)
{
  return HT_SIZE (tree);
}


void rtree_add (rtree_t * tree, void * node)
{
  HT_INSERT (rtree, tree, node);
}


void * rtree_get (rtree_t * tree, void * node)
{
  rnode_t * hit = HT_FIND (rtree, tree, node);
  return hit ? hit -> foo : NULL;
}


void rtree_del (rtree_t * tree, void * node)
{
  rnode_t * hit = HT_FIND (rtree, tree, node);
  if (hit)
    HT_REMOVE (rtree, tree, hit);
}


bool rtree_has (rtree_t * tree, char * key)
{
  return rtree_get (tree, key);
}


void rtree_foreach (rtree_t * tree, rtree_each_f * fn, void * data)
{
  rnode_t ** obj;
  HT_FOREACH (obj, rtree, tree)
    fn (data);
}


char ** rtree_keys (rtree_t * tree)
{
  char ** keys = calloc (rtree_count (tree) + 1, sizeof (char *));
  unsigned i = 0;
  rnode_t ** k;
  HT_FOREACH (k, rtree, tree)
    keys [i ++] = (* k) -> key;
  return keys;
}


void ** rtree_vals (rtree_t * tree)
{
  void ** vals = calloc (rtree_count (tree) + 1, sizeof (void *));
  unsigned i = 0;
  rnode_t ** k;
  HT_FOREACH (k, rtree, tree)
    vals [i ++] = (* k) -> foo;
  return vals;
}
