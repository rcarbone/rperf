/* System headers */
#include <stdlib.h>
#include <string>
using namespace std;


/* key && val */
typedef char rkey_t;
typedef void * rval_t;


/* The implementation */
#include "htrie_map.h"
using namespace tsl;


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
typedef tsl::htrie_map<rkey_t, rval_t> rtrie_t;
#include "rtrie-api.h"

/* Node definition */
#include "nodes.h"
#include "safe.h"


rtrie_t * rtrie_alloc (void)
{
  return new rtrie_t;
}


void rtrie_free (rtrie_t * trie)
{
  delete trie;
}


void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data)
{
  rtrie_t::iterator it;
  for (it = trie -> begin (); it != trie -> end (); ++ it)
    if (fn)
      fn (data);
}


unsigned rtrie_count (rtrie_t * trie)
{
  return trie -> size ();
}


void rtrie_add (rtrie_t * trie, void * node)
{
  rnode_t * pair = (rnode_t *) node;

  char * key = pair -> key;
  void * val = pair -> foo;

  // (* trie) [key] = val;
  trie -> insert (key, val);
}


void * rtrie_get (rtrie_t * trie, void * node)
{
  rnode_t * pair = (rnode_t *) node;
  rtrie_t::iterator it = trie -> find (pair -> key);

  return it != trie -> end () ? node : NULL;
}


void rtrie_del (rtrie_t * trie, void * node)
{
  rnode_t * pair = (rnode_t *) node;
  trie -> erase (pair -> key);
}
