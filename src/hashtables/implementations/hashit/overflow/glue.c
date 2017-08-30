/* System headers */
#include <stdio.h>
#include <string.h>

/* The implementation */
#include "hashit.c"
#include "hfunctions.c"

/* librhash - an abstract C library over real hash tables */
typedef struct shash_t rht_t;
#include "rht-api.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static int cmp_str (void * a, void * b)
{
  return strcmp (a, b);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  return hashit_create (size, 0, NULL, cmp_str, OVERFLOW_H);
}


void rht_free (rht_t * ht)
{
  hashit_destroy (ht);
}


void rht_clear (rht_t * ht)
{
  unsigned n = hashit_size (ht);
  char ** keys = (char **) hashit_getkeys (ht);
  unsigned i;
  for (i = 0; i < n; i ++)
    hashit_delete (ht, keys [i]);
  free (keys);
}


unsigned rht_count (rht_t * ht)
{
  return hashit_size (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  hashit_insert (ht, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  return hashit_lookup (ht, key);
}


void rht_del (rht_t * ht, char * key)
{
  hashit_delete (ht, key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  unsigned n = hashit_size (ht);
  unsigned i;
  for (i = 0; i < n; i ++)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  unsigned n = hashit_size (ht);
  char ** keys = calloc (n + 1, sizeof (char *));
  char ** k = (char **) hashit_getkeys (ht);
  unsigned i;
  for (i = 0; i < n; i ++)
    keys [i] = k [i];
  free (k);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  unsigned n = hashit_size (ht);
  void ** vals = calloc (n + 1, sizeof (void *));
  void ** v = (void **) hashit_getvalues (ht);
  unsigned i;
  for (i = 0; i < n; i ++)
    vals [i] = v [i];
  free (v);
  return vals;
}
