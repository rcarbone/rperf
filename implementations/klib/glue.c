/* System headers */
#include <stdio.h>

/* The implementation */
#include "khash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rht-hashers.h"

/* Generates typedefs for hash table and inline functions */
KHASH_INIT(rhash, char *, void *, 1, rht_python_hash, kh_str_hash_equal)

/* librhash - an abstract C library over real hash tables */
typedef khash_t(rhash) rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  return kh_init (rhash);
}


void rhash_free (rhash_t * ht)
{
  kh_destroy (rhash, ht);
}


void rhash_clear (rhash_t * ht)
{
  kh_clear (rhash, ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return kh_size (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  int ret;
  khiter_t k = kh_put (rhash, ht, key, & ret);
  kh_value (ht, k) = val;
}


void * rhash_get (rhash_t * ht, char * key)
{
  khiter_t k = kh_get (rhash, ht, key);
  return k == kh_end (ht) ? NULL : kh_value (ht, k);
}


void rhash_del (rhash_t * ht, char * key)
{
  khiter_t k = kh_get (rhash, ht, key);
  kh_del (rhash, ht, k);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return kh_get (rhash, ht, key) != kh_end (ht);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  khint_t k;
  for (k = kh_begin (ht); k != kh_end (ht); k ++)
    {
      if (! kh_exist (ht, k))
	continue;
      fn (data);
    }
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  khiter_t k;
  for (k = kh_begin (ht); k != kh_end (ht); k ++)
    {
      if (! kh_exist (ht, k))
	continue;
      keys [i ++] = (char *) kh_key (ht, k);
    }
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  khiter_t k;
  for (k = kh_begin (ht); k < kh_end (ht); k ++)
    {
      if (! kh_exist (ht, k))
	continue;
      vals [i ++] = (void *) kh_value (ht, k);
    }
  return vals;
}
