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
#include "rht-api.h"

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
  khiter_t it = kh_put (rht, ht, key, & ret);
  kh_value (ht, it) = val;
}


void * rht_get (rht_t * ht, char * key)
{
  khiter_t it = kh_get (rht, ht, key);
  return it == kh_end (ht) ? NULL : kh_value (ht, it);
}


void rht_del (rht_t * ht, char * key)
{
  khiter_t it = kh_get (rht, ht, key);
  kh_del (rht, ht, it);
}


bool rht_has (rht_t * ht, char * key)
{
  return kh_get (rht, ht, key) != kh_end (ht);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  khint_t it;
  for (it = kh_begin (ht); it != kh_end (ht); it ++)
    {
      if (! kh_exist (ht, it))
	continue;
      fn (data);
    }
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  khiter_t it;
  for (it = kh_begin (ht); it != kh_end (ht); it ++)
    {
      if (! kh_exist (ht, it))
	continue;
      keys [i ++] = (char *) kh_key (ht, it);
    }
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  khiter_t it;
  for (it = kh_begin (ht); it != kh_end (ht); it ++)
    {
      if (! kh_exist (ht, it))
	continue;
      vals [i ++] = (void *) kh_value (ht, it);
    }
  return vals;
}
