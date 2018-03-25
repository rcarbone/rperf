#pragma once


/* Some implementations require fields embedded into the elements to save memory/cycles */


/* tommyds - Andrea Mazzoleni */
#include "tommytree.h"

/* libevent - Niels Provos's implementation of hashtable */
#include "ht-internal.h"

/* libevent - Niels Provos's implementation of splay/red-black tree */
#include "tree.h"


/* Object definition */
typedef struct robj robj_t;
struct robj
{
  char     * key;
  unsigned   val;
  robj_t   * foo;

  unsigned   len;

  /* Required by tommyds implementation to make the structure tree-able */
  tommy_tree_obj _tommy_;

  /* required by libevent implementation to make the structure tree-able */
  HT_ENTRY(robj) _levt_;

  /* required by libevent implementation to make the structure tree-able */
  SPLAY_ENTRY(robj) _lesp_;

  /* required by libevent implementation to make the structure tree-able */
  RB_ENTRY(robj) _lerb_;
};


/* API */

#ifdef __cplusplus
extern "C" {
#endif

robj_t * mkobj (unsigned key);
void rmobj (robj_t * obj);
int cmpobj (const void * n1, const void * n2);
robj_t ** mkobjs (unsigned argc);
void rmobjs (robj_t * argv []);
robj_t ** mkmiss (unsigned argc);

#ifdef __cplusplus
}
#endif /* __cplusplus */
