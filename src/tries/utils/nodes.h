#pragma once


/* Some implementations require fields embedded into the elements to save memory/cycles */


/* tommyds - Andrea Mazzoleni */
#include "tommytypes.h"


/* Trie Node definition */
typedef struct rnode rnode_t;
typedef struct rnode
{
  char     * key;
  unsigned   val;
  rnode_t  * foo;

  /* Required by tommyds implementation to make the structure trie-able */
  tommy_node tommy;

} rnode_t;


/* API */
rnode_t * mknode (unsigned key);
void rmnode (rnode_t * node);
rnode_t ** mknodes (unsigned argc);
void rmnodes (rnode_t * argv []);
