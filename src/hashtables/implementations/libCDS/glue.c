/* System headers */
#include <stdio.h>

/* The implementation */
#include "hash_map.c"

/* librhash - an abstract C library over real hash tables */
typedef HashMap rht_t;
#include "rht-api.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rht-hashers.h"


static unsigned hashfunc (void * key)
{
  return rht_python_hash (key);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  HashMap * ht = HashMapInit ();
  HashMapSetHash (ht, hashfunc);
  return ht;
}


void rht_free (rht_t * ht)
{
  HashMapDeinit (ht);
}


void rht_clear (rht_t * ht)
{
  Pair * pair;
  HashMapFirst (ht);
  while ((pair = HashMapNext (ht)))
    HashMapRemove (ht, pair -> key);
}


unsigned rht_count (rht_t * ht)
{
  return HashMapSize (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  HashMapPut (ht, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  return HashMapGet (ht, key);
}


void rht_del (rht_t * ht, char * key)
{
  HashMapRemove (ht, key);
}


bool rht_has (rht_t * ht, char * key)
{
  return HashMapContain (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  HashMapFirst (ht);
  while (HashMapNext (ht))
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  Pair * pair;
  HashMapFirst (ht);
  unsigned i = 0;
  while ((pair = HashMapNext (ht)))
    keys [i ++] = pair -> key;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  Pair * pair;
  HashMapFirst (ht);
  unsigned i = 0;
  while ((pair = HashMapNext (ht)))
    vals [i ++] = pair -> value;
  return vals;
}
