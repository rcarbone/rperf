/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include "sparsehash/sparse_hash_map"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* librhash - an abstract C library over real hash tables */
#include "rht-hashers.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
class hashfunc
{
 public:
  unsigned operator () (char * key) const { return rht_python_hash (key); }
};

typedef google::sparse_hash_map <char *, void *, hashfunc> rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = new rht_t;
  ht -> set_deleted_key ((char *) "-1");
  return ht;
}


void rht_free (rht_t * ht)
{
  delete ht;
}


void rht_clear (rht_t * ht)
{
  ht -> clear ();
}


unsigned rht_count (rht_t * ht)
{
  return ht -> size ();
}


void rht_set (rht_t * ht, char * key, void * val)
{
  ht -> insert (std::make_pair (key, val));
}


void * rht_get (rht_t * ht, char * key)
{
  rht_t::iterator k = ht -> find (key);
  return k != ht -> end () ? k -> second : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  ht -> erase (key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  rht_t::iterator k;
  for (k = ht -> begin (); k != ht -> end (); k ++)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = (char **) calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  rht_t::iterator k;
  for (k = ht -> begin (); k != ht -> end (); k ++)
    keys [i ++] = k -> first;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = (void **) calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  rht_t::iterator k;
  for (k = ht -> begin (); k != ht -> end (); k ++)
    vals [i ++] = k -> second;
  return vals;
}
