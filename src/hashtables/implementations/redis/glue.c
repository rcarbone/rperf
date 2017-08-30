/* System headers */
#include <stdio.h>

/* The implementation */
#include "dict.c"
#include "zmalloc.c"
#include "siphash.c"

/* librht - an abstract C library over real hash tables */
typedef dict rht_t;
#include "rht-api.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static uint64_t str_hash_f (const void * keyp)
{
  return rht_python_hash (keyp);
}


static int str_cmp_f (void * privdata, const void * key1, const void * key2)
{
  return ! strcmp (key1, key2);
}


static dictType redis =
{
  str_hash_f,                    /* hash function */
  NULL,                          /* key dup */
  NULL,                          /* val dup */
  str_cmp_f,                     /* key compare */
  NULL,                          /* key destructor */
  NULL                           /* val destructor */
};

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  return dictCreate (& redis, NULL);
}


void rht_free (rht_t * ht)
{
  dictRelease (ht);
}


void rht_clear (rht_t * ht)
{
  dictIterator * it = dictGetSafeIterator (ht);
  dictEntry * e;
  for (e = dictNext (it); e; e = dictNext (it))
    dictDelete (ht, e -> key);
  dictReleaseIterator (it);
}


unsigned rht_count (rht_t * ht)
{
  return dictSize (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  dictAdd (ht, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  dictEntry * e = dictFind (ht, key);
  return e ? e -> v . val : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  dictDelete (ht, key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  dictIterator * it = dictGetIterator (ht);
  dictEntry * e;
  for (e = dictNext (it); e; e = dictNext (it))
    fn (data);
  dictReleaseIterator (it);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  dictIterator * it = dictGetIterator (ht);
  dictEntry * e;
  for (e = dictNext (it); e; e = dictNext (it))
    keys [i ++] = e -> key;
  dictReleaseIterator (it);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  dictIterator * it = dictGetIterator (ht);
  dictEntry * e;
  for (e = dictNext (it); e; e = dictNext (it))
    vals [i ++] = e -> v . val;
  dictReleaseIterator (it);
  return vals;
}
