/* System headers */
#include <stdlib.h>


/* The implementation */
#include "hat-trie.c"
#include "ahtable.c"
#include "misc.c"
#include "murmurhash3.c"


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
typedef hattrie_t rtrie_t;
#include "rtrie-api.h"

/* Node definition */
#include "nodes.h"
#include "safe.h"


rtrie_t * rtrie_alloc (void)
{
  return hattrie_create ();
}


void rtrie_free (rtrie_t * trie)
{
  hattrie_free  (trie);
}


unsigned rtrie_count (rtrie_t * trie)
{
  return hattrie_size (trie);
}


void rtrie_add (rtrie_t * trie, void * node)
{
  hattrie_get (trie, ((rnode_t *) node) -> key, strlen (((rnode_t *) node) -> key));
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  return hattrie_tryget (trie, ((rnode_t *) node) -> key, strlen (((rnode_t *) node) -> key));
}


void rtrie_del (rtrie_t * trie, void * node)
{
  hattrie_del (trie, ((rnode_t *) node) -> key, strlen (((rnode_t *) node) -> key));
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  unsigned i = hattrie_size (trie);
  while (i --)
    if (fn)
      fn (data);
}
