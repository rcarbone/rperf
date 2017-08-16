/* System headers */
#include <stdio.h>

/* The implementation */
#include "apr_hash.c"

/* librht - an abstract C library over real hash tables */
typedef apr_hash_t rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#define apr_hash_del(x, y, z)  apr_hash_set(x, y, z, NULL)


static unsigned int hashfunc (const char * key, apr_ssize_t * klen)
{
  return rht_python_hash (key);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  return apr_hash_make_custom (NULL, hashfunc);
}


void rht_free (rht_t * ht)
{
  apr_hash_done (ht);
}


void rht_clear (rht_t * ht)
{
  apr_hash_clear (ht);
}


unsigned rht_count (rht_t * ht)
{
  return apr_hash_count (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  apr_hash_set (ht, key, strlen (key), val);
}


void * rht_get (rht_t * ht, char * key)
{
  return apr_hash_get (ht, key, strlen (key));
}


void rht_del (rht_t * ht, char * key)
{
  apr_hash_del (ht, key, strlen (key));
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    keys [i ++] = (char *) apr_hash_this_key (k);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    vals [i ++] = (void *) apr_hash_this_val (k);
  return vals;
}
