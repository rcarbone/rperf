/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The implementation */
#include "HashMap.h"
using namespace rigtorp;

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* librhash - an abstract C library over real hash tables */
#include "rht-hashers.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
struct hasher
{
  unsigned operator () (char * key) { return rht_python_hash (key); }
};


struct equaler
{
  inline bool operator () (char * s1, char * s2) { return ! strcmp (s1, s2); }
};


/* librhash - an abstract C library over real hash tables */
typedef rigtorp::HashMap<char *, void *, hasher, equaler> rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  return new rhash_t (0, (char *) "");
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
