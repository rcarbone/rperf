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
DEFINE_CHAINHASH (rhash, char *, void *, ULIB_ISMAP, custom_hash_fn, eq_fn, cmp_fn)

/* librhash - an abstract C library over real hash tables */
typedef chainhash_t(rhash) rhash_t;
#include "rhash.h"


static unsigned chainhash_size (rhash_t * ht)
{
  unsigned size = 0;
  chainhash_itr_t(rhash) it = chainhash_begin (rhash, ht);
  while (! chainhash_end (it))
    {
      size ++;
      if (chainhash_advance (rhash, & it))
	it . entry = NULL;
    }
  return size;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  return chainhash_init (rhash, size);
}


void rhash_free (rhash_t * ht)
{
  chainhash_destroy (rhash, ht);
}


void rhash_clear (rhash_t * ht)
{
  chainhash_clear (rhash, ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return chainhash_size (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  chainhash_itr_t(rhash) it = chainhash_set (rhash, ht, key);
  if (! chainhash_end (it))
    chainhash_value (rhash, it) = val;
}


void * rhash_get (rhash_t * ht, char * key)
{
  chainhash_itr_t(rhash) it = chainhash_get (rhash, ht, key);
  return it . entry ? chainhash_value (rhash, it) : NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  chainhash_itr_t(rhash) it = chainhash_get (rhash, ht, key);
  if (! chainhash_end (it))
    chainhash_del (rhash, it);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  chainhash_itr_t(rhash) it = chainhash_begin (rhash, ht);
  while (! chainhash_end (it))
    {
      fn (data);
      if (chainhash_advance (rhash, & it))
	it . entry = NULL;
    }
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  chainhash_itr_t(rhash) it = chainhash_begin (rhash, ht);
  while (! chainhash_end (it))
    {
      keys [i ++] = chainhash_key (it);
      if (chainhash_advance (rhash, & it))
	it . entry = NULL;
    }
  return keys;
}

void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  chainhash_itr_t(rhash) it = chainhash_begin (rhash, ht);
  while (! chainhash_end (it))
    {
      vals [i ++] = chainhash_value (rhash, it);
      if (chainhash_advance (rhash, & it))
	it . entry = NULL;
    }
  return vals;
}

