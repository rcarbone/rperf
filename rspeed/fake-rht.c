/* System headers */
#include <stdlib.h>


/* librhash - an abstract C library over real hash table implementations */
typedef struct rhash rht_t;
#include "rht.h"


typedef struct rhash
{
  unsigned dummy;
} rht_t;


rht_t * rht_alloc (unsigned size)
{
  return (rht_t *) calloc (1, sizeof (rht_t));
}


void rht_free (rht_t * ht)
{
  if (ht)
    free (ht);
}


void rht_clear (rht_t * ht)
{
}


unsigned rht_count (rht_t * ht)
{
  return ht ? 1 : 0;
}


void rht_set (rht_t * ht, char * key, void * val)
{
}


void * rht_get (rht_t * ht, char * key)
{
  return NULL;
}


void rht_del (rht_t * ht, char * key)
{
}


bool rht_has (rht_t * ht, char * key)
{
  return false;
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
}


char ** rht_keys (rht_t * ht)
{
  return NULL;
}


void ** rht_vals (rht_t * ht)
{
  return NULL;
}
