/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* The implementation */
static bool _antiresize_recursion = false;
#include "hashmap.hpp"

using namespace container;

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* librhash - an abstract C library over real hash tables */
#include "rht-hashers.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
class hashfunc
{
 public:
  unsigned operator () (char * key) const { return rht_python_hash (key); }
};

/* librhash - an abstract C library over real hash tables */
typedef container::hash_map<char *, void *, hashfunc> rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  return new rhash_t;
}


void rhash_free (rhash_t * ht)
{
  delete ht;
}


void rhash_clear (rhash_t * ht)
{
  ht -> clear ();
}


unsigned rhash_count (rhash_t * ht)
{
  return ht -> size ();
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  ht -> insert (std::pair <char *, void *> (key, val));
}


void * rhash_get (rhash_t * ht, char * key)
{
  rhash_t::iterator k = ht -> find (key);
  return k != ht -> end () ? k -> second : NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  ht -> erase (key);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  rhash_t::iterator it = ht -> begin ();
  for (; it != ht -> end (); ++ it)
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = (char **) calloc (rhash_count (ht) + 1, sizeof (char *));
  rhash_t::iterator it = ht -> begin ();
  unsigned i = 0;
  for (; it != ht -> end (); ++ it)
    keys [i ++] = it -> first;
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = (void **) calloc (rhash_count (ht) + 1, sizeof (void *));
  rhash_t::iterator it = ht -> begin ();
  unsigned i = 0;
  for (; it != ht -> end (); ++ it)
    vals [i ++] = it -> second;
  return vals;
}
