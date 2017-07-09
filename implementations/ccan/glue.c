/* System headers */
#include <stdio.h>

/* The implementation */
#include "3rdparty/htable.c"

/* librhash - an abstract C library over real hash tables */
typedef struct htable rht_t;
#include "rht.h"
#include "datasets.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* Hash string keys using an external helper function */
static size_t hash_str (const void * key, void * unused)
{
  return rht_python_hash (key);
}


/* Compare 2 string keys for equality */
static bool cmp_key (const void * obj, void * key)
{
  return ! strcmp (((robj_t *) obj) -> skey, key);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (rht_t));
  htable_init (ht, hash_str, NULL);
  return ht;
}


void rht_free (rht_t * ht)
{
  htable_clear (ht);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  htable_clear (ht);
}


unsigned rht_count (rht_t * ht)
{
  return ht -> elems;
}


void rht_set (rht_t * ht, char * key, void * val)
{
  htable_add (ht, rht_python_hash (key), val);
}


void * rht_get (rht_t * ht, char * key)
{
  struct htable_iter it;
  void * v;

  for (v = htable_first (ht, & it); v; v = htable_next (ht, & it))
    if (cmp_key (v, key))
      return v;
  return NULL;
}


void rht_del (rht_t * ht, char * key)
{
  struct htable_iter it;
  void * obj;
  for (obj = htable_first (ht, & it); obj; obj = htable_next (ht, & it))
    if (cmp_key (obj, key))
      htable_delval (ht, & it);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  struct htable_iter it;
  void * obj;
  for (obj = htable_first (ht, & it); obj; obj = htable_next (ht, & it))
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  struct htable_iter it;
  void * obj;
  for (obj = htable_first (ht, & it); obj; obj = htable_next (ht, & it))
    keys [i ++] = (char *) ((robj_t *) obj) -> skey;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  struct htable_iter it;
  void * obj;
  for (obj = htable_first (ht, & it); obj; obj = htable_next (ht, & it))
    vals [i ++] = obj;
  return vals;
}
