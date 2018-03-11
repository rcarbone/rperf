/* System headers */
#include <stdlib.h>


/* The implementation */
#include "tr_tree.c"
#include "tree_common.c"
#include "dict.c"


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
typedef tr_tree rtrie_t;
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
  return tr_tree_new (rnodescmp, NULL);
}


void rtrie_free (rtrie_t * trie)
{
  tr_tree_free (trie, NULL);
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  unsigned i = tr_tree_count (trie);
  while (i --)
    if (fn)
      fn (data);
}


unsigned rtrie_count (rtrie_t * trie)
{
  return tr_tree_count (trie);
}


void rtrie_add (rtrie_t * trie, void * node)
{
  tr_tree_insert (trie, node);
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  return tr_tree_search (trie, node);
}


void rtrie_del (rtrie_t * trie, void * node)
{
  tr_tree_remove (trie, node);
}
