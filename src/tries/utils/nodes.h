#pragma once


/* Some implementations require fields embedded into the elements to save memory/cycles */

/* Trie Node definition */
typedef struct rnode rnode_t;
typedef struct rnode
{
  char     * key;
  unsigned   foo;
  rnode_t  * bar;

} rnode_t;


/* API */
rnode_t * mknode (unsigned key);
void rmnode (rnode_t * node);
rnode_t ** mknodes (unsigned argc);
void rmnodes (rnode_t * argv []);
