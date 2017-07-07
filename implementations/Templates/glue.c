/* System headers */
#include <stdio.h>

/* The implementation */
#include "xxx.c"

/* librhash - an abstract C library over real hash tables */
typedef xxx_t rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rht-hashers.h"


static unsigned int hashfunc (char * key)
{
  return rht_python_hash (key);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rhash_t * rhash_alloc (unsigned size)
{
  rhash_t * ht = calloc (1, sizeof (* ht));
  return ht;
}


void rhash_free (rhash_t * ht)
{
  free (ht);
}


void rhash_clear (rhash_t * ht)
{
}


unsigned rhash_count (rhash_t * ht)
{
  return 0;
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
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (void *));
  return vals;
}
