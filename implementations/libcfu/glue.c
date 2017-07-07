/* System headers */
#include <stdio.h>

/* The implementation */
#include "cfuhash.c"

/* librhash - an abstract C library over real hash tables */
typedef cfuhash_table_t rhash_t;
#include "rhash.h"
#include "varrays.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

typedef struct
{
  void (* fn) (void *);
  void * data;

} func_t;


static int myforeach (void * key, size_t key_size, void * data, size_t data_size, void * arg)
{
  func_t * func = arg;
  if (func)
    func -> fn (func -> data);
  return 0;
}


/* Callback to iterate over the hash table to add a key */
static int addkey (void * key, size_t key_size, void * data, size_t data_size, void * keys)
{
  char ** k = * (char ***) keys;
  k = (char **) vamore ((void **) k, (char *) key);
  * (char ***) keys = k;
  return 0;
}


/* Callback to iterate over the hash table to add a val */
static int addval (void * key, size_t key_size, void * data, size_t data_size, void * vals)
{
  void ** v = * (void ***) vals;
  v = vamore (v, data);
  * (void ***) vals = v;
  return 0;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  return cfuhash_new ();
}


void rhash_free (rhash_t * ht)
{
  cfuhash_destroy (ht);
}


void rhash_clear (rhash_t * ht)
{
  cfuhash_clear (ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return cfuhash_num_entries (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  cfuhash_put (ht, key, val);
}


void * rhash_get (rhash_t * ht, char * key)
{
  return cfuhash_get (ht, key);
}


void rhash_del (rhash_t * ht, char * key)
{
  cfuhash_delete (ht, key);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return cfuhash_exists (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  cfuhash_foreach (ht, myforeach, & fun);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  cfuhash_foreach (ht, addkey, & keys);
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (void *));
  cfuhash_foreach (ht, addval, & vals);
  return vals;
}
