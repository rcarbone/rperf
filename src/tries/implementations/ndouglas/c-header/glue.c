/* System headers */
#include <stdlib.h>


/* The implementation */
#include "nedtrie.h"


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
#define NEED_RTRIE_TYPEDEF
#include "rtrie-api.h"

/* Node definition */
#include "nodes.h"
#include "safe.h"


/* Forward */
static unsigned fun (const rnode_t * node);


/* Trie definition */
NEDTRIE_HEAD(rtrie, rnode);


/* Generates prototypes and inline functions */
NEDTRIE_GENERATE(static, rtrie, rnode, ned, fun, NEDTRIE_NOBBLEZEROS(rtrie))


/* Key access function */
static unsigned fun (const rnode_t * node)
{
  return node -> val;
}


rtrie_t * rtrie_alloc (void)
{
  rtrie_t * trie = calloc (1, sizeof (* trie));

  /* Initialize trie */
  NEDTRIE_INIT (trie);

  return trie;
}


void rtrie_free (rtrie_t * trie)
{
  safefree (trie);
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  rnode_t * node;
  NEDTRIE_FOREACH (node, rtrie, trie)
  {
    if (fn)
      fn (data);
  }
}


unsigned rtrie_count (rtrie_t * trie)
{
  return NEDTRIE_COUNT (trie);
}


void rtrie_add (rtrie_t * trie, void * node)
{
  NEDTRIE_INSERT (rtrie, trie, node);
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  return NEDTRIE_FIND (rtrie, trie, node);
}


void rtrie_del (rtrie_t * trie, void * node)
{
  if (NEDTRIE_FIND (rtrie, trie, node))
    NEDTRIE_REMOVE (rtrie, trie, node);
}
