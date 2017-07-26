
#pragma once


#define DEFAULT_OBJS 1959


/* libevent - Niels Provos */
#include "ht-internal.h"

/* tommyds - Andrea Mazzoleni */
#include "tommytypes.h"

/* uthash - Troy D. Hanson */
#include "uthash.h"


/* Object definition */
typedef struct robj robj_t;
typedef struct robj
{
  unsigned   ukey;      /* sequential */
  unsigned   umiss;

  unsigned   ukey_2;    /* dense      */
  unsigned   umiss_2;

  unsigned   ukey_3;    /* sparse     */
  unsigned   umiss_3;

  unsigned   rand_2;
  unsigned   rand_3;

  char     * skey;
  char     * smiss;
  char     * skey_2;
  char     * skey_3;

  robj_t   * pkey;

  unsigned   uval;
  char     * sval;
  robj_t   * pval;

  /* hash values evaluated at inizialization time to increase performance */
  unsigned   uhash;
  unsigned   shash;
  void     * phash;

  /* required by libevent implementation to make the structure hash-able */
  HT_ENTRY(robj) _levt_;

  /* required by tommyds implementation to make the structure hash-able */
  tommy_node _tommy_;

  /* required by uthash implementation to make the structure hash-able */
  UT_hash_handle hh;           /* field must be named 'hh' in order to use the convenience macros */

} robj_t;


#ifdef __cplusplus
extern "C" {
#endif


/* Public functions */
robj_t * mkobj (unsigned key, unsigned val, unsigned miss, unsigned dense, unsigned sparse);
void rmobj (robj_t * obj);
robj_t ** mkobjs (unsigned argc);
void rmobjs (robj_t * argv []);


#ifdef __cplusplus
}
#endif
