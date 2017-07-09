/* System headers */
#include <stdio.h>
#include <stdlib.h>


#include "rht.h"
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
  ht -> some -> clear ();
}


unsigned rht_count (rht_t * ht)
{
  return ht -> some -> elements ();
}


void rht_set (rht_t * ht, char * key, void * val)
{
  some_t::Insert it = ht -> some -> findForAdd (key);
  if (it . found ())
    ht -> some -> add (it, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  some_t::Insert it = ht -> some -> findForAdd (key);
  return it . found () ? it -> key : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  some_t::Insert it = ht -> some -> findForAdd (key);
  if (it . found ())
    ht -> some -> remove (it);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  some_t::iterator it = ht -> some -> iter ();
  for (ht -> some -> iter (); ! it . empty (); it . next ())
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = (char **) calloc (rht_count (ht) + 1, sizeof (char *));
  some_t::iterator it = ht -> some -> iter ();
  unsigned i = 0;
  for (ht -> some -> iter (); ! it . empty (); it . next ())
    keys [i ++] = it -> key;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = (void **) calloc (rht_count (ht) + 1, sizeof (void *));
  some_t::iterator it = ht -> some -> iter ();
  unsigned i = 0;
  for (ht -> some -> iter (); ! it . empty (); it . next ())
    vals [i ++] = it -> value;
  return vals;
}
