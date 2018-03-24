#pragma once


/* Some implementations require fields embedded into the elements to save memory/cycles */


/* tommyds - Andrea Mazzoleni */
#include "tommytree.h"


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
};


/* API */
rnode_t * mknode (unsigned key);
void rmnode (rnode_t * node);
int cmpnode (const void * n1, const void * n2);
rnode_t ** mknodes (unsigned argc);
void rmnodes (rnode_t * argv []);
rnode_t ** mkmiss (unsigned argc);
