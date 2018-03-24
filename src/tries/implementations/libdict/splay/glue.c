/* System headers */
#include <stdlib.h>


/* The implementation */
#include "sp_tree.c"
#include "tree_common.c"
#include "dict.c"


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
typedef sp_tree rtrie_t;
#include "rtrie-api.h"

/* Node definition */
#include "nodes.h"


rtrie_t * rtrie_alloc (void)
{
  return sp_tree_new (rnodescmp);
}


void rtrie_free (rtrie_t * trie)
{
  sp_tree_free (trie, NULL);
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  unsigned i = sp_tree_count (trie);
  while (i --)
    if (fn)
      fn (data);
}


unsigned rtrie_count (rtrie_t * trie)
{
  return sp_tree_count (trie);
}


void rtrie_add (rtrie_t * trie, void * node)
{
  sp_tree_insert (trie, node);
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  return sp_tree_search (trie, node) ? node : NULL;
}


void rtrie_del (rtrie_t * trie, void * node)
{
  sp_tree_remove (trie, node);
}
