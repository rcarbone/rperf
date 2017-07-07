/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The implementation */
#include <map>

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rhashfuncs.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
class hashfunc
{
 public:
  bool operator () (const char * k1, const char * k2) const { return strcmp (k1, k2); }
};


/* librhash - an abstract C library over real hash tables */
typedef std::map <char *, void *, hashfunc> rhash_t;
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
  ht -> insert (std::make_pair (key, val));
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
  rhash_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = (char **) calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  rhash_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    keys [i ++] = it -> first;
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = (void **) calloc (rhash_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  rhash_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    vals [i ++] = it -> second;
  return vals;
}
