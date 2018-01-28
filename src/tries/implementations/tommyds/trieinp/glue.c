/* System headers */
#include <stdlib.h>


/* The implementation */
#include "tommylist.c"
#include "tommytrieinp.c"


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
typedef tommy_trie_inplace rtrie_t;
#include "rtrie-api.h"

#include "nodes.h"
#include "safe.h"


rtrie_t * rtrie_alloc (void)
{
  rtrie_t * trie = calloc (1, sizeof (* trie));
  tommy_trie_inplace_init (trie);
  return trie;
}


void rtrie_free (rtrie_t * trie)
{
  safefree (trie);
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  unsigned i = tommy_trie_inplace_count (trie);
  while (i --)
    if (fn)
      fn (data);
}


unsigned rtrie_count (rtrie_t * trie)
{
  return tommy_trie_inplace_count (trie);
}


void rtrie_add (rtrie_t * trie, void * node)
{
  tommy_trie_inplace_insert (trie, & ((rnode_t *) node) -> andrea, node, ((rnode_t *) node) -> val);
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  return tommy_trie_inplace_search (trie, ((rnode_t *) node) -> val);
}


void rtrie_del (rtrie_t * trie, void * node)
{
  tommy_trie_inplace_remove (trie, ((rnode_t *) node) -> val);
}
