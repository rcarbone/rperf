/* System headers */
#include <stdio.h>
#include <stdlib.h>


#include "rhash.h"
#include "hashfuns.h"


/* The implementation */
#include "amtl/am-hashmap.h"
#include "amtl/am-string.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */

using namespace ke;

struct Policy
{
  static inline uint32_t hash (char * key)
  {
    return python_hash (key);
  }

  static inline bool matches (char * find, char * key)
  {
    // return key . compare (find) == 0;
    return ! strcmp (find, key);
  }
};


typedef ke::HashMap<char *, void *, Policy> some_t;


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
  ht -> some -> clear ();
}


unsigned rhash_count (rhash_t * ht)
{
  return ht -> some -> elements ();
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  some_t::Insert it = ht -> some -> findForAdd (key);
  if (it . found ())
    ht -> some -> add (it, key, val);
}


void * rhash_get (rhash_t * ht, char * key)
{
  some_t::Insert it = ht -> some -> findForAdd (key);
  return it . found () ? it -> key : NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  some_t::Insert it = ht -> some -> findForAdd (key);
  if (it . found ())
    ht -> some -> remove (it);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  some_t::iterator it = ht -> some -> iter ();
  for (ht -> some -> iter (); ! it . empty (); it . next ())
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = (char **) calloc (rhash_count (ht) + 1, sizeof (char *));
  some_t::iterator it = ht -> some -> iter ();
  unsigned i = 0;
  for (ht -> some -> iter (); ! it . empty (); it . next ())
    keys [i ++] = it -> key;
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = (void **) calloc (rhash_count (ht) + 1, sizeof (void *));
  some_t::iterator it = ht -> some -> iter ();
  unsigned i = 0;
  for (ht -> some -> iter (); ! it . empty (); it . next ())
    vals [i ++] = it -> value;
  return vals;
}
