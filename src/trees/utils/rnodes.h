#pragma once


/* Some implementations require fields embedded into the elements to save memory/cycles */


/* tommyds - Andrea Mazzoleni */
#include "tommytree.h"

/* libevent - Niels Provos's implementation of hashtable */
#include "ht-internal.h"

/* libevent - Niels Provos's implementation of splay/red-black tree */
#include "tree.h"


/* Tree Node definition */
typedef struct rnode rnode_t;
struct rnode
{
  char     * key;
  unsigned   val;
  rnode_t  * foo;
  unsigned   len;

  /* Required by tommyds implementation to make the structure tree-able */
  tommy_tree_node tommy;

  /* required by libevent implementation to make the structure tree-able */
  HT_ENTRY(rnode) _levt_;

  /* required by libevent implementation to make the structure tree-able */
  SPLAY_ENTRY(rnode) _lest_;

  /* required by libevent implementation to make the structure tree-able */
  RB_ENTRY(rnode) _lerb_;
};


/* API */
rnode_t * mknode (unsigned key);
void rmnode (rnode_t * node);
int cmpnode (const void * n1, const void * n2);
rnode_t ** mknodes (unsigned argc);
void rmnodes (rnode_t * argv []);
rnode_t ** mkmiss (unsigned argc);
