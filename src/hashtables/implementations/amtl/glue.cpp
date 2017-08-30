/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The implementation */
#include "amtl/am-hashmap.h"
using namespace ke;

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* librhash - an abstract C library over real hash tables */
#include "rht-hashers.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
struct Policy
{
  static inline uint32_t hash (char * key)
  {
    return rht_python_hash (key);
  }

  static inline bool matches (char * find, char * key)
  {
    return ! strcmp (find, key);
  }
};


/* librhash - an abstract C library over real hash tables */
typedef ke::HashMap<char *, void *, Policy> rht_t;
#include "rht-api.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = new rht_t;
  ht -> init ();
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
  return ht -> elements ();
}


void rht_set (rht_t * ht, char * key, void * val)
{
  rht_t::Insert it = ht -> findForAdd (key);
  ht -> add (it, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  rht_t::Result it = ht -> find (key);
  return it . found () ? it -> value : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  rht_t::Insert it = ht -> findForAdd (key);
  if (it . found ())
    ht -> remove (it);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  rht_t::iterator it = ht -> iter ();
  for (ht -> iter (); ! it . empty (); it . next ())
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = (char **) calloc (rht_count (ht) + 1, sizeof (char *));
  rht_t::iterator it = ht -> iter ();
  unsigned i = 0;
  for (ht -> iter (); ! it . empty (); it . next ())
    keys [i ++] = it -> key;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = (void **) calloc (rht_count (ht) + 1, sizeof (void *));
  rht_t::iterator it = ht -> iter ();
  unsigned i = 0;
  for (ht -> iter (); ! it . empty (); it . next ())
    vals [i ++] = it -> value;
  return vals;
}
