/* System headers */
#include <stdlib.h>

/* GNU libc */
#include <search.h>


/* librhash - an abstract C library over real hash table implementations */
typedef struct rhash rhash_t;
#include "rhash.h"


typedef struct rhash
{
  unsigned count;

} rhash_t;


rhash_t * rhash_alloc (unsigned size)
{
  hcreate (size);
  return (rhash_t *) calloc (1, sizeof (rhash_t));
}


void rhash_free (rhash_t * ht)
{
  hdestroy ();
  if (ht)
    free (ht);
}


void rhash_clear (rhash_t * ht)
{
  hdestroy ();
  ht -> count = 0;
}


unsigned rhash_count (rhash_t * ht)
{
  return ht -> count;
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  ENTRY e;

  e . key  = key;
  e . data = val;

  hsearch (e, ENTER);
  ht -> count ++;
}


void * rhash_get (rhash_t * ht, char * key)
{
  ENTRY e;
  ENTRY * ep;
  e . key = key;

  ep = hsearch (e, FIND);

  return ep -> data;
}


void rhash_del (rhash_t * ht, char * key)
{
  ENTRY e;
  ENTRY * ep;
  e . key = key;

  ep = hsearch (e, FIND);
  if (ep)
    ht -> count --;
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
