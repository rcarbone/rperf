/* System headers */
#include <stdio.h>

/* The implementation */
#include "hash_open_prot.h"
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
DEFINE_OPENHASH (rht, char *, void *, ULIB_ISMAP, hash_str_fn, eq_str_fn)

/* librht - an abstract C library over real hash tables */
typedef openhash_t(rht) rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  return openhash_init (rht);
}


void rht_free (rht_t * ht)
{
  openhash_destroy (rht, ht);
}


void rht_clear (rht_t * ht)
{
  openhash_clear (rht, ht);
}


unsigned rht_count (rht_t * ht)
{
  return openhash_size (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  int ret;
  oh_iter_t k = openhash_set (rht, ht, key, & ret);
  openhash_value (ht, k) = val;
}


void * rht_get (rht_t * ht, char * key)
{
  oh_iter_t k = openhash_get (rht, ht, key);
  return k == openhash_end (ht) ? NULL : openhash_value (ht, k);
}


void rht_del (rht_t * ht, char * key)
{
  oh_iter_t k = openhash_get (rht, ht, key);
  openhash_del (rht, ht, k);
}


bool rht_has (rht_t * ht, char * key)
{
  return openhash_get (rht, ht, key) != openhash_end (ht);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  oh_iter_t k;
  for (k = openhash_begin (ht); k != openhash_end (ht); k ++)
    {
      if (! openhash_exist (ht, k))
	continue;
      fn (data);
    }
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  oh_iter_t k;
  for (k = openhash_begin (ht); k != openhash_end (ht); k ++)
    {
      if (! openhash_exist (ht, k))
	continue;
      keys [i ++] = (char *) openhash_key (ht, k);
    }
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  oh_iter_t k;
  for (k = openhash_begin (ht); k < openhash_end (ht); k ++)
    {
      if (! openhash_exist (ht, k))
	continue;
      vals [i ++] = (void *) openhash_value (ht, k);
    }
  return vals;
}
