#pragma once


/* Some implementations require fields embedded into the elements to save memory/cycles */


/* tommyds - Andrea Mazzoleni */
#include "tommytrie.h"
#include "tommytrieinp.h"

/* nedtrie - Niall Douglas */
#if defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wunused-variable"
#endif
#include "nedtrie.h"


/* Trie Node definition */
typedef struct rnode rnode_t;
struct rnode
{
  char     * key;
  unsigned   val;
  rnode_t  * foo;

  /* Required by tommyds implementation to make the structure trie-able */
  tommy_trie_node tommy;

  /* Required by tommyds implementation to make the structure trie-able */
  tommy_trie_inplace_node andrea;

  /* Required by nedtrie implementation to make the structure trie-able */
  NEDTRIE_ENTRY(rnode) ned;
};


/* API */
rnode_t * mknode (unsigned key);
void rmnode (rnode_t * node);
rnode_t ** mknodes (unsigned argc);
void rmnodes (rnode_t * argv []);
rnode_t ** mkmiss (unsigned argc);
