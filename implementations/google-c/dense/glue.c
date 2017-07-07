/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#define Table(x) Dense##x           /* Google Dense tables */
#include "libchash.c"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* librhash - an abstract C library over real hash tables */
typedef struct HashTable rhash_t;
#include "rhash.h"

#define DONTCOPY 0

static unsigned HashCount (struct HashTable * ht)
{
  return ht -> cItems;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rhash_t * rhash_alloc (unsigned size)
{
  return AllocateHashTable (sizeof (void *), DONTCOPY);
}


void rhash_free (rhash_t * ht)
{
  FreeHashTable (ht);
}


void rhash_clear (rhash_t * ht)
{
  ClearHashTable (ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return HashCount (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  HashInsert (ht, (ulong) key, (ulong) val);
}


void * rhash_get (rhash_t * ht, char * key)
{
  HTItem * obj = HashFind (ht, (ulong) key);
  return obj ? (void *) obj -> data : NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  HashDelete (ht, (ulong) key);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  HTItem * obj = HashFirstBucket (ht);
  while (obj)
    {
      fn (data);
      obj = HashNextBucket (ht);
    }
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = (char **) calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  HTItem * obj = HashFirstBucket (ht);
  while (obj)
    {
      keys [i ++] = (char *) obj -> key;
      obj = HashNextBucket (ht);
    }
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = (void **) calloc (rhash_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  HTItem * obj = HashFirstBucket (ht);
  while (obj)
    {
      vals [i ++] = (void *) obj -> key;
      obj = HashNextBucket (ht);
    }
  return vals;
}
