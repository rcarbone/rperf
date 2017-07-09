/* System headers */
#include <stdio.h>

/* The implementation */
#include "cfuhash.c"

/* librhash - an abstract C library over real hash tables */
typedef cfuhash_table_t rht_t;
#include "rht.h"
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

rht_t * rht_alloc (unsigned size)
{
  return cfuhash_new ();
}


void rht_free (rht_t * ht)
{
  cfuhash_destroy (ht);
}


void rht_clear (rht_t * ht)
{
  cfuhash_clear (ht);
}


unsigned rht_count (rht_t * ht)
{
  return cfuhash_num_entries (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  cfuhash_put (ht, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  return cfuhash_get (ht, key);
}


void rht_del (rht_t * ht, char * key)
{
  cfuhash_delete (ht, key);
}


bool rht_has (rht_t * ht, char * key)
{
  return cfuhash_exists (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  cfuhash_foreach (ht, myforeach, & fun);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  cfuhash_foreach (ht, addkey, & keys);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  cfuhash_foreach (ht, addval, & vals);
  return vals;
}
