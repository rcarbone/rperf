/* System headers */
#include <stdio.h>

/* The implementation */
#include "3rdparty/htable.c"

/* librhash - an abstract C library over real hash tables */
typedef struct htable rhash_t;
#include "rhash.h"
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

rhash_t * rhash_alloc (unsigned size)
{
  rhash_t * ht = calloc (1, sizeof (rhash_t));
  htable_init (ht, hash_str, NULL);
  return ht;
}


void rhash_free (rhash_t * ht)
{
  htable_clear (ht);
  free (ht);
}


void rhash_clear (rhash_t * ht)
{
  htable_clear (ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return ht -> elems;
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  htable_add (ht, rht_python_hash (key), val);
}


void * rhash_get (rhash_t * ht, char * key)
{
  struct htable_iter it;
  void * v;

  for (v = htable_first (ht, & it); v; v = htable_next (ht, & it))
    if (cmp_key (v, key))
      return v;
  return NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  struct htable_iter it;
  void * obj;
  for (obj = htable_first (ht, & it); obj; obj = htable_next (ht, & it))
    if (cmp_key (obj, key))
      htable_delval (ht, & it);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  struct htable_iter it;
  void * obj;
  for (obj = htable_first (ht, & it); obj; obj = htable_next (ht, & it))
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  struct htable_iter it;
  void * obj;
  for (obj = htable_first (ht, & it); obj; obj = htable_next (ht, & it))
    keys [i ++] = (char *) ((robj_t *) obj) -> skey;
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  struct htable_iter it;
  void * obj;
  for (obj = htable_first (ht, & it); obj; obj = htable_next (ht, & it))
    vals [i ++] = obj;
  return vals;
}
