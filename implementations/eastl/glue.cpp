/* System headers */
#include <stdio.h>
#include <stdlib.h>


#include "rht.h"
#include "hashfuns.h"


/* The implementation */
// #include "EASTL/hash_map.h"
#include "hashtable.cpp"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
class hashfunc
{
 public:
  unsigned operator () (char * key) const { return python_hash (key); }
};


// typedef eastl::hash_map<char *, void *, hashfunc> some_t;
typedef void * some_t;


struct rhash
{
  some_t * some;
};


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = (rht_t * ) calloc (1, sizeof (* ht));
  ht -> some = new some_t;
  return ht;
}


void rht_free (rht_t * ht)
{
  delete ht -> some;
  free (ht);
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
