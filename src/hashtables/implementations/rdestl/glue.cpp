/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include "hash_map.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* librhash - an abstract C library over real hash tables */
#include "rht-hashers.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
class hashfunc
{
 public:
  unsigned operator () (char * key) const { return rht_python_hash (key); }
};


struct eqstr
{
  inline bool operator () (char * key1, char * key2) const { return ! strcmp (key1, key2); }
};


struct hasher
{
  inline rde::hash_value_t operator () (char * key) const { return rht_python_hash (key); }
};


/* librhash - an abstract C library over real hash tables */
typedef rde::hash_map<char *, void *, hasher, 6, eqstr> rht_t;
#include "rht-api.h"

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
  ht -> clear ();
}


unsigned rht_count (rht_t * ht)
{
  return ht -> size ();
}


void rht_set (rht_t * ht, char * key, void * val)
{
  ht -> insert (rde::pair <char *, void *> (key, val));
}


void * rht_get (rht_t * ht, char * key)
{
  rht_t::iterator it = ht -> find (key);
  return it != ht -> end () ? it -> second : NULL;
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
  rht_t::iterator it = ht -> begin ();
  for (; it != ht -> end (); it ++)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = (char **) calloc (rht_count (ht) + 1, sizeof (char *));
  rht_t::iterator it = ht -> begin ();
  unsigned i = 0;
  for (; it != ht -> end (); it ++)
    keys [i ++] = it -> first;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = (void **) calloc (rht_count (ht) + 1, sizeof (void *));
  rht_t::iterator it = ht -> begin ();
  unsigned i = 0;
  for (; it != ht -> end (); it ++)
    vals [i ++] = it -> second;
  return vals;
}
