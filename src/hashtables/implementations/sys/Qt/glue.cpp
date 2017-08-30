/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#undef QT_BOOTSTRAPPED
#include <QtCore/QHash>

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* librhash - an abstract C library over real hash tables */
#include "rht-hashers.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
// static quint32 qHash (const char * & key) { return rht_python_hash (key); }

typedef QHash <char *, void *> rht_t;
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
  ht -> insert (key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  rht_t::iterator it = ht -> find (key);
  return it != ht -> end () ? it . value () : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  ht -> remove (key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  rht_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = (char **) calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  rht_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    keys [i ++] = (char *) it . key ();
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = (void **) calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  rht_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    vals [i ++] = it . value ();
  return vals;
}
