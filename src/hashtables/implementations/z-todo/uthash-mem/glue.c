/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include "uthash.h"

#define NEED_RHT_TYPEDEF
#include "rht-api.h"
#include "vargv.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Private Object Type required to make it hash-able */
typedef struct utobj utobj_t;
typedef struct utobj
{
  char    * key;
  utobj_t * val;

  UT_hash_handle hh;     /* field must be named hh in order to use the convenience macros */

} utobj_t;


struct rht
{
  utobj_t * uthash;
};


static utobj_t * mkpair (char * key, void * val)
{
  utobj_t * obj = calloc (1, sizeof (* obj));
  obj -> key = strdup (key);
  obj -> val = val;
  return obj;
}


static void rmpair (utobj_t * obj)
{
  free (obj -> key);
  free (obj);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (* ht));
  ht -> uthash = NULL;                       /* Important! initialize to NULL */
  return ht;
}


void rht_free (rht_t * ht)
{
  char ** keys = rht_keys (ht);
  char ** k = keys;
  while (* k)
    rht_del (ht, * k ++);
  if (keys)
    free (keys);
  HASH_CLEAR (hh, ht -> uthash);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  char ** keys = rht_keys (ht);
  char ** k = keys;
  while (* k)
    rht_del (ht, * k ++);
  if (keys)
    free (keys);
  HASH_CLEAR (hh, ht -> uthash);
  ht -> uthash = NULL;                       /* Important! initialize to NULL */
}


unsigned rht_count (rht_t * ht)
{
  return HASH_COUNT (ht -> uthash);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  utobj_t * obj = mkpair (key, val);
  HASH_ADD_KEYPTR (hh, ht -> uthash, key, strlen (key), obj);
}


void * rht_get (rht_t * ht, char * key)
{
  utobj_t * hit;
  HASH_FIND_STR (ht -> uthash, key, hit);
  return hit ? hit -> val : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  utobj_t * hit;
  HASH_FIND_STR (ht -> uthash, key, hit);
  if (hit)
    {
      HASH_DEL (ht -> uthash, hit);
      rmpair (hit);
    }
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  utobj_t * it;
  for (it = ht -> uthash; it; it = (utobj_t *) (it -> hh . next))
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  utobj_t * it;
  for (it = ht -> uthash; it; it = (utobj_t *) (it -> hh . next))
    keys = (char **) vamore ((void **) keys, it -> hh . key);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (char *));
  utobj_t * it;
  for (it = ht -> uthash; it; it = (utobj_t *) (it -> hh . next))
    vals = vamore (vals, rht_get (ht, it -> hh . key));
  return vals;
}
