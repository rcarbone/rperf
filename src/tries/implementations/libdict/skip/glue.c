/* System headers */
#include <stdlib.h>


/* The implementation */
#include "skiplist.c"
#include "tree_common.c"
#include "dict.c"


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
typedef skiplist rtrie_t;
#include "rtrie-api.h"

/* Node definition */
#include "nodes.h"
#include "safe.h"


static int rnodescmp (const void * n1, const void * n2)
{
  return strcmp (((rnode_t *) n1) -> key, ((rnode_t *) n2) -> key);
}


rtrie_t * rtrie_alloc (void)
{
  return skiplist_new (rnodescmp, MAX_LINK);
}


void rtrie_free (rtrie_t * trie)
{
  skiplist_free (trie, NULL);
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  unsigned i = skiplist_count (trie);
  while (i --)
    if (fn)
      fn (data);
}


unsigned rtrie_count (rtrie_t * trie)
{
  return skiplist_count (trie);
}


void rtrie_add (rtrie_t * trie, void * node)
{
  skiplist_insert (trie, node);
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  return skiplist_search (trie, node);
}


void rtrie_del (rtrie_t * trie, void * node)
{
  skiplist_remove (trie, node);
}
