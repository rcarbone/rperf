/* System headers */
#include <stdio.h>
#include <stdlib.h>


#include "rhash.h"
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


rhash_t * rhash_alloc (unsigned size)
{
  rhash_t * ht = (rhash_t * ) calloc (1, sizeof (* ht));
  ht -> some = new some_t;
  return ht;
}


void rhash_free (rhash_t * ht)
{
  delete ht -> some;
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
  char ** keys = (char **) calloc (rhash_count (ht) + 1, sizeof (char *));
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = (void **) calloc (rhash_count (ht) + 1, sizeof (void *));
  return vals;
}
