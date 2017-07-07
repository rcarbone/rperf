/* System headers */
#include <stdio.h>
#include <string.h>

/* The implementation */
#include "hashit.c"
#include "hfunctions.c"

/* librhash - an abstract C library over real hash tables */
typedef struct shash_t rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static int cmp_str (void * a, void * b)
{
  return strcmp (a, b);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  return hashit_create (size, 0, NULL, cmp_str, OVERFLOW_H);
}


void rhash_free (rhash_t * ht)
{
  hashit_destroy (ht);
}


void rhash_clear (rhash_t * ht)
{
  unsigned n = hashit_size (ht);
  char ** keys = (char **) hashit_getkeys (ht);
  unsigned i;
  for (i = 0; i < n; i ++)
    hashit_delete (ht, keys [i]);
  free (keys);
}


unsigned rhash_count (rhash_t * ht)
{
  return hashit_size (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  hashit_insert (ht, key, val);
}


void * rhash_get (rhash_t * ht, char * key)
{
  return hashit_lookup (ht, key);
}


void rhash_del (rhash_t * ht, char * key)
{
  hashit_delete (ht, key);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  unsigned n = hashit_size (ht);
  unsigned i;
  for (i = 0; i < n; i ++)
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
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


void ** rhash_vals (rhash_t * ht)
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
