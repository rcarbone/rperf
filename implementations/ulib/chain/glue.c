/* System headers */
#include <stdio.h>

/* The implementation */
#include "hash_chain_prot.h"
#define ULIB_ISMAP  1

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rht-hashers.h"

static int custom_hash_fn (char * key)
{
  return rht_python_hash (key);
}


static int eq_fn (char * k1, char * k2)
{
  return ! strcmp (k1, k2);
}


static int cmp_fn (char * k1, char * k2)
{
  return ! strcmp (k1, k2);
}


/* Generates typedefs for hash table and inline functions */
DEFINE_CHAINHASH (rht, char *, void *, ULIB_ISMAP, custom_hash_fn, eq_fn, cmp_fn)

/* librht - an abstract C library over real hash tables */
typedef chainhash_t(rht) rht_t;
#include "rht.h"


static unsigned chainhash_size (rht_t * ht)
{
  unsigned size = 0;
  chainhash_itr_t(rht) it = chainhash_begin (rht, ht);
  while (! chainhash_end (it))
    {
      size ++;
      if (chainhash_advance (rht, & it))
	it . entry = NULL;
    }
  return size;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  return chainhash_init (rht, size);
}


void rht_free (rht_t * ht)
{
  chainhash_destroy (rht, ht);
}


void rht_clear (rht_t * ht)
{
  chainhash_clear (rht, ht);
}


unsigned rht_count (rht_t * ht)
{
  return chainhash_size (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  chainhash_itr_t(rht) it = chainhash_set (rht, ht, key);
  if (! chainhash_end (it))
    chainhash_value (rht, it) = val;
}


void * rht_get (rht_t * ht, char * key)
{
  chainhash_itr_t(rht) it = chainhash_get (rht, ht, key);
  return it . entry ? chainhash_value (rht, it) : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  chainhash_itr_t(rht) it = chainhash_get (rht, ht, key);
  if (! chainhash_end (it))
    chainhash_del (rht, it);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  chainhash_itr_t(rht) it = chainhash_begin (rht, ht);
  while (! chainhash_end (it))
    {
      fn (data);
      if (chainhash_advance (rht, & it))
	it . entry = NULL;
    }
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  chainhash_itr_t(rht) it = chainhash_begin (rht, ht);
  while (! chainhash_end (it))
    {
      keys [i ++] = chainhash_key (it);
      if (chainhash_advance (rht, & it))
	it . entry = NULL;
    }
  return keys;
}

void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  chainhash_itr_t(rht) it = chainhash_begin (rht, ht);
  while (! chainhash_end (it))
    {
      vals [i ++] = chainhash_value (rht, it);
      if (chainhash_advance (rht, & it))
	it . entry = NULL;
    }
  return vals;
}

