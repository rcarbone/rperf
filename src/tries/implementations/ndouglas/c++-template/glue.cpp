/* System headers */
#include <stdlib.h>


#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wwrite-strings"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#endif


/* The implementation */
#define NEDTRIE_ENABLE_STL_CONTAINERS 1
#include "nedtrie.h"
using namespace nedtries;


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
typedef trie_map<unsigned, unsigned> rtrie_t;
#include "rtrie-api.h"

/* Node definition */
#include "nodes.h"
#include "safe.h"


rtrie_t * rtrie_alloc (void)
{
  rtrie_t * trie = new rtrie_t;

  return trie;
}


void rtrie_free (rtrie_t * trie)
{
  delete trie;
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  rtrie_t::iterator it;
  for (it = trie -> begin (); it != trie -> end (); ++ it)
    {
      if (fn)
	fn (data);
    }
}


unsigned rtrie_count (rtrie_t * trie)
{
  return trie -> size ();
}


void rtrie_add (rtrie_t * trie, void * node)
{
  rnode_t * pair = (rnode_t *) node;
  (* trie) [pair -> val] = pair -> val;
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  rnode_t * pair = (rnode_t *) node;
  rtrie_t::iterator it = trie -> find (pair -> val);

  return it != trie -> end () ? node : NULL;
}


void rtrie_del (rtrie_t * trie, void * node)
{
  rnode_t * pair = (rnode_t *) node;
  rtrie_t::iterator it = trie -> find (pair -> val);
  if (it != trie -> end ())
    trie -> erase (it);
}
