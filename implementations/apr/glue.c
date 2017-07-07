/* System headers */
#include <stdio.h>

/* The implementation */
#include "apr_hash.c"

/* librhash - an abstract C library over real hash tables */
typedef apr_hash_t rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#define apr_hash_del(x, y, z)  apr_hash_set(x, y, z, NULL)


static unsigned int hashfunc (const char * key, apr_ssize_t * klen)
{
  return rht_python_hash (key);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rhash_t * rhash_alloc (unsigned size)
{
  return apr_hash_make_custom (NULL, hashfunc);
}


void rhash_free (rhash_t * ht)
{
  apr_hash_done (ht);
}


void rhash_clear (rhash_t * ht)
{
  apr_hash_clear (ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return apr_hash_count (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  apr_hash_set (ht, key, strlen (key), val);
}


void * rhash_get (rhash_t * ht, char * key)
{
  return apr_hash_get (ht, key, strlen (key));
}


void rhash_del (rhash_t * ht, char * key)
{
  apr_hash_del (ht, key, strlen (key));
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    keys [i ++] = (char *) apr_hash_this_key (k);
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    vals [i ++] = (void *) apr_hash_this_val (k);
  return vals;
}
