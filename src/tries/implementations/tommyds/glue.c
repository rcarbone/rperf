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
  tommy_trie * trie = calloc (1, sizeof (* trie));
  tommy_allocator_init (& alloc, TOMMY_TRIE_BLOCK_SIZE, TOMMY_TRIE_BLOCK_SIZE);
  tommy_trie_init (trie, & alloc);
  return trie;
}


void rtrie_free (rtrie_t * trie)
{
  safefree (trie);
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
#if defined(ROCCO)
  tommy_node * node = tommy_trie_bucket (trie, value);
  while (node)
    {
      if (fn)
	fn (data);
      node = node -> next;
    }
#endif /* ROCCO */
}


unsigned rtrie_count (rtrie_t * trie)
{
  return tommy_trie_count (trie);
}


void rtrie_add (rtrie_t * trie, void * node)
{
#if defined(ROCCO)
  tommy_trie_insert (trie, & node -> tommy, node, node -> value);
#endif /* ROCCO */
}


void * rtrie_get (rtrie_t * trie, void * node)
{
#if defined(ROCCO)
  tommy_node * node = tommy_search (node -> value);
  if (node)
    return node;
#endif /* ROCCO */
  return NULL;
}


void rtrie_del (rtrie_t * trie, void * node)
{
#if defined(ROCCO)
  tommy_list_remove (trie, value);
#endif /* ROCCO */
}
