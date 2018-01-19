/* System headers */
#include <stdlib.h>


/* Project headers */
#include "vargv.h"
#include "nodes.h"
#include "safe.h"


/* This implementation uses a dynamic array of void pointers just as example */
typedef struct
{
  rnode_t ** trie;

} rtrie_t;


/* librtrie - an abstract C library over real trie implementations */
#include "rtrie-api.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rtrie_t * rtrie_alloc (void)
{
  return calloc (1, sizeof (rtrie_t));
}


void rtrie_free (rtrie_t * trie)
{
  arrclear (trie -> trie, NULL);
  safefree (trie);
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  rnode_t ** node = trie -> trie;
  while (node && * node ++)
    if (fn)
      fn (data);
}


unsigned rtrie_count (rtrie_t * trie)
{
  return (arrlen (trie -> trie));
}


void rtrie_add (rtrie_t * trie, void * node)
{
  trie -> trie = arrmore (trie -> trie, node, rnode_t);
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  rnode_t ** n = trie -> trie;
  while (n && * n)
    if (* n == node)
      return node;
    else
      n ++;
  return NULL;
}


void rtrie_del (rtrie_t * trie, void * node)
{
  if (rtrie_get (trie, node))
    trie -> trie = arrless (trie -> trie, node, rnode_t, NULL);
}
