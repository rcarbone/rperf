/* System headers */
#include <stdlib.h>


/* The implementation */
#include "tommyalloc.c"
#include "tommylist.c"
#include "tommytrie.c"


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
typedef tommy_trie rtrie_t;
#include "rtrie-api.h"

#include "nodes.h"
#include "safe.h"


static tommy_allocator alloc;


rtrie_t * rtrie_alloc (void)
{
  rtrie_t * trie = calloc (1, sizeof (* trie));
  tommy_allocator_init (& alloc, TOMMY_TRIE_BLOCK_SIZE, TOMMY_TRIE_BLOCK_SIZE);
  tommy_trie_init (trie, & alloc);
  return trie;
}


void rtrie_free (rtrie_t * trie)
{
  tommy_allocator_done (& alloc);
  safefree (trie);
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  unsigned i = tommy_trie_count (trie);
  while (i --)
    if (fn)
      fn (data);
}


unsigned rtrie_count (rtrie_t * trie)
{
  return tommy_trie_count (trie);
}


void rtrie_add (rtrie_t * trie, void * node)
{
  tommy_trie_insert (trie, & ((rnode_t *) node) -> tommy, node, ((rnode_t *) node) -> val);
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  return tommy_trie_search (trie, ((rnode_t *) node) -> val);
}


void rtrie_del (rtrie_t * trie, void * node)
{
  tommy_trie_remove (trie, ((rnode_t *) node) -> val);
}
