/* System headers */
#include <stdlib.h>


/* librhash - an abstract C library over real hash table implementations */
typedef struct rhash rhash_t;
#include "rhash.h"


typedef struct rhash
{
  unsigned dummy;
} rhash_t;


rhash_t * rhash_alloc (unsigned size)
{
  return (rhash_t *) calloc (1, sizeof (rhash_t));
}


void rhash_free (rhash_t * ht)
{
  if (ht)
    free (ht);
}


void rhash_clear (rhash_t * ht)
{
}


unsigned rhash_count (rhash_t * ht)
{
  return ht ? 1 : 0;
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
}


void * rhash_get (rhash_t * ht, char * key)
{
  return NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
}


bool rhash_has (rhash_t * ht, char * key)
{
  return false;
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
}


char ** rhash_keys (rhash_t * ht)
{
  return NULL;
}


void ** rhash_vals (rhash_t * ht)
{
  return NULL;
}
