/* System headers */
#include <stdio.h>

/* The implementation */
#include "khash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rht-hashers.h"

/* Generates typedefs for hash table and inline functions */
KHASH_INIT(rht, char *, void *, 1, rht_python_hash, kh_str_hash_equal)

/* librht - an abstract C library over real hash tables */
typedef khash_t(rht) rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  return kh_init (rht);
}


void rht_free (rht_t * ht)
{
  kh_destroy (rht, ht);
}


void rht_clear (rht_t * ht)
{
  kh_clear (rht, ht);
}


unsigned rht_count (rht_t * ht)
{
  return kh_size (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  int ret;
  khiter_t k = kh_put (rht, ht, key, & ret);
  kh_value (ht, k) = val;
}


void * rht_get (rht_t * ht, char * key)
{
  khiter_t k = kh_get (rht, ht, key);
  return k == kh_end (ht) ? NULL : kh_value (ht, k);
}


void rht_del (rht_t * ht, char * key)
{
  khiter_t k = kh_get (rht, ht, key);
  kh_del (rht, ht, k);
}


bool rht_has (rht_t * ht, char * key)
{
  return kh_get (rht, ht, key) != kh_end (ht);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  khint_t k;
  for (k = kh_begin (ht); k != kh_end (ht); k ++)
    {
      if (! kh_exist (ht, k))
	continue;
      fn (data);
    }
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
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


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
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
