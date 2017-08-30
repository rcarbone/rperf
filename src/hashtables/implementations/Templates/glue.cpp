/* System headers */
#include <stdio.h>

/* The implementation */
#include <xxx>

/* librhash - an abstract C library over real hash tables */
typedef xxx_t rht_t;
#include "rht-api.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rhashfuns.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
class hashfunc
{
 public:
  unsigned operator () (char * key) const { return python_hash (key); }
};


typedef xxx::yyy<char *, void *, hashfunc> rht_t;

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  return new rht_t;
}


void rht_free (rht_t * ht)
{
  delete ht;
}


void rht_clear (rht_t * ht)
{
}


unsigned rht_count (rht_t * ht)
{
  return 0;
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
  char ** keys = (char **) calloc (rht_count (ht) + 1, sizeof (char *));
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = (void **) calloc (rht_count (ht) + 1, sizeof (void *));
  return vals;
}
