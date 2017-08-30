/* System headers */
#include <stdio.h>
#include <string.h>

/* The implementation */
#include "hash_functions.c"
#include "hash_table.c"

/* librht - an abstract C library over real hash tables */
typedef ght_hash_table_t rht_t;
#include "rht-api.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static ght_uint32_t hashfunc (ght_hash_key_t * key)
{
  return rht_python_hash (key -> p_key);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = ght_create (size);
  ght_set_hash (ht, hashfunc);
  return ht;
}


void rht_free (rht_t * ht)
{
  ght_finalize (ht);
}


void rht_clear (rht_t * ht)
{
  ght_iterator_t it;
  const void * key;
  void * val;
  for (val = ght_first (ht, & it, & key); val; val = ght_next (ht, & it, & key))
    ght_remove (ht, strlen (key) + 1, key);
}


unsigned rht_count (rht_t * ht)
{
  return ght_size (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  ght_insert (ht, val, strlen (key) + 1, key);
}


void * rht_get (rht_t * ht, char * key)
{
  return ght_get (ht, strlen (key) + 1, key);
}


void rht_del (rht_t * ht, char * key)
{
  ght_remove (ht, strlen (key) + 1, key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  ght_iterator_t it;
  const void * key;
  void * val;
  for (val = ght_first (ht, & it, & key); val; val = ght_next (ht, & it, & key))
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  ght_iterator_t it;
  const void * key;
  void * val;
  for (val = ght_first (ht, & it, & key); val; val = ght_next (ht, & it, & key))
    keys [i ++] = (char *) key;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  ght_iterator_t it;
  const void * key;
  void * val;
  for (val = ght_first (ht, & it, & key); val; val = ght_next (ht, & it, & key))
    vals [i ++] = val;
  return vals;
}
