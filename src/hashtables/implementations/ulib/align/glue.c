/* System headers */
#include <stdio.h>

/* The implementation */
#include "ulib/hash_align_prot.h"
#define ULIB_ISMAP  1

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rht-hashers.h"

static int hash_str_fn (char * key)
{
  return rht_python_hash (key);
}


static int eq_str_fn (char * k1, char * k2)
{
  return ! strcmp (k1, k2);
}


/* Generates typedefs for hash table and inline functions */
DEFINE_ALIGNHASH (rht, char *, void *, ULIB_ISMAP, hash_str_fn, eq_str_fn)

/* librht - an abstract C library over real hash tables */
typedef alignhash_t(rht) rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  return alignhash_init (rht);
}


void rht_free (rht_t * ht)
{
  alignhash_destroy (rht, ht);
}


void rht_clear (rht_t * ht)
{
  alignhash_clear (rht, ht);
}


unsigned rht_count (rht_t * ht)
{
  return alignhash_size (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  int ret;
  ah_iter_t it = alignhash_set (rht, ht, key, & ret);
  alignhash_value (ht, it) = val;
}


void * rht_get (rht_t * ht, char * key)
{
  ah_iter_t it = alignhash_get (rht, ht, key);
  return it == alignhash_end (ht) ? NULL : alignhash_value (ht, it);
}


void rht_del (rht_t * ht, char * key)
{
  ah_iter_t it = alignhash_get (rht, ht, key);
  alignhash_del (rht, ht, it);
}


bool rht_has (rht_t * ht, char * key)
{
  return alignhash_get (rht, ht, key) != alignhash_end (ht);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  ah_iter_t it;
  for (it = alignhash_begin (ht); it != alignhash_end (ht); it ++)
    {
      if (! alignhash_exist (ht, it))
	continue;
      fn (data);
    }
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  ah_iter_t it;
  for (it = alignhash_begin (ht); it != alignhash_end (ht); it ++)
    {
      if (! alignhash_exist (ht, it))
	continue;
      keys [i ++] = (char *) alignhash_key (ht, it);
    }
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  ah_iter_t it;
  for (it = alignhash_begin (ht); it < alignhash_end (ht); it ++)
    {
      if (! alignhash_exist (ht, it))
	continue;
      vals [i ++] = (void *) alignhash_value (ht, it);
    }
  return vals;
}
