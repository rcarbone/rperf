/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#define Table(x) Dense##x           /* Google Dense tables */
#include "libchash.c"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* librhash - an abstract C library over real hash tables */
typedef struct HashTable rht_t;
#include "rht.h"

#define DONTCOPY 0

static unsigned HashCount (struct HashTable * ht)
{
  return ht -> cItems;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  return AllocateHashTable (sizeof (void *), DONTCOPY);
}


void rht_free (rht_t * ht)
{
  FreeHashTable (ht);
}


void rht_clear (rht_t * ht)
{
  ClearHashTable (ht);
}


unsigned rht_count (rht_t * ht)
{
  return HashCount (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  HashInsert (ht, (ulong) key, (ulong) val);
}


void * rht_get (rht_t * ht, char * key)
{
  HTItem * obj = HashFind (ht, (ulong) key);
  return obj ? (void *) obj -> data : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  HashDelete (ht, (ulong) key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  HTItem * obj = HashFirstBucket (ht);
  while (obj)
    {
      fn (data);
      obj = HashNextBucket (ht);
    }
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = (char **) calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  HTItem * obj = HashFirstBucket (ht);
  while (obj)
    {
      keys [i ++] = (char *) obj -> key;
      obj = HashNextBucket (ht);
    }
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = (void **) calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  HTItem * obj = HashFirstBucket (ht);
  while (obj)
    {
      vals [i ++] = (void *) obj -> key;
      obj = HashNextBucket (ht);
    }
  return vals;
}
