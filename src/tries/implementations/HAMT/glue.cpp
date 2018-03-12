/* System headers */
#include <stdlib.h>
using namespace std;


/* key && val */
typedef char * rkey_t;
typedef void * rval_t;


/* The implementation */
#include "HashTrie.h"
namespace HAMT { using ::THashTrie; }
using namespace HAMT;


/* Project headers */

/* librtrie - an abstract C library over real trie implementations */
typedef HAMT::THashTrie<rkey_t, rval_t> rtrie_t;
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
#if defined(ROCCO)
  rtrie_t::iterator it;
  for (it = trie -> begin (); it != trie -> end (); ++ it)
    if (fn)
      fn (data);
#endif /* ROCCO */
}


unsigned rtrie_count (rtrie_t * trie)
{
#if defined(ROCCO)
  return trie -> size ();
#else
  return 0;
#endif /* ROCCO */
}


void rtrie_add (rtrie_t * trie, void * node)
{
  rnode_t * pair = (rnode_t *) node;

  char * key = pair -> key;
  void * val = pair -> foo;

#if defined(ROCCO)
  (* trie) [key] = val;
  trie -> insert (key, val);
#endif /* ROCCO */
}


void * rtrie_get (rtrie_t * trie, void * node)
{
#if defined(ROCCO)
  rnode_t * pair = (rnode_t *) node;
  rtrie_t::iterator it = trie -> find (pair -> key);

  return it != trie -> end () ? node : NULL;
#else
  return NULL;
#endif /* ROCCO */
}


void rtrie_del (rtrie_t * trie, void * node)
{
#if defined(ROCCO)
  rnode_t * pair = (rnode_t *) node;
  trie -> erase (pair -> key);
#endif /* ROCCO */
}
