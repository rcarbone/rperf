/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include "ulib/hash_chain.h"
using namespace ulib;

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rht-hashers.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
class hashfunc
{
 public:
  unsigned operator () (char * key) const { return rht_python_hash (key); }
};

/* librhash - an abstract C library over real hash tables */
typedef ulib::chain_hash_map <char *, void *, hashfunc> rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rhash_t * rhash_alloc (unsigned size)
{
  return new rhash_t (size);
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
  ht -> insert (key, val);
}


void * rhash_get (rhash_t * ht, char * key)
{
  rhash_t::iterator it = ht -> find (key);
  return it != ht -> end () ? it . value () : NULL;
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
  rhash_t::iterator k;
  for (k = ht -> begin (); k != ht -> end (); k ++)
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = (char **) calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  rhash_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    keys [i ++] = it . key ();
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = (void **) calloc (rhash_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  rhash_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    vals [i ++] = it . value ();
  return vals;
}
