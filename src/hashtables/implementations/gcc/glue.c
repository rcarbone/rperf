/* System headers */
#include <stdio.h>

/* The implementation */
#include "hashtab.c"

/* librht - an abstract C library over real hash tables */
#define NEED_RHT_TYPEDEF
#include "rht-api.h"
#include "datasets.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

typedef struct rht
{
  htab_t gcc;

} rht_t;


/* Typedef used in the foreach callback */
typedef struct
{
  void (* f) (void *);
  void * data;

} func_t;


/* Typedef used in the keys/vals callbacks */
typedef struct
{
  char ** keys;
  void ** vals;
  unsigned i;

} kvcb_t;


static hashval_t hash_str_fn (const void * key)
{
  return rht_python_hash (key);
}


static int eq_str_fn (const void * obj, const void * key)
{
  return ! strcmp (((robj_t *) obj) -> skey, key);
}


static int foreach_cb (void ** slot, void * arg)
{
  func_t * func = arg;
  func -> f (func -> data);
  return 1;
}


/* Callback to iterate over the hash table to add a key */
static int addkey_cb (void ** slot, void * arg)
{
  kvcb_t * kv = arg;
  kv -> keys [kv -> i ++] = (* (robj_t **) slot) -> skey;
  return 1;
}


/* Callback to iterate over the hash table to add a val */
static int addval_cb (void ** slot, void * arg)
{
  kvcb_t * kv = arg;
  kv -> vals [kv -> i ++] = (* (robj_t **) slot) -> pval;
  return 1;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (* ht));
  ht -> gcc = htab_create (size * 2, hash_str_fn, eq_str_fn, NULL);   /* Bug! double the size due to bug while finding the keys */
  return ht;
}


void rht_free (rht_t * ht)
{
  htab_delete (ht -> gcc);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  htab_empty (ht -> gcc);
}


unsigned rht_count (rht_t * ht)
{
  return htab_elements (ht -> gcc);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  void ** elem = htab_find_slot (ht -> gcc, key, INSERT);
  if (elem)
    * elem = val;
}


void * rht_get (rht_t * ht, char * key)
{
  return htab_find (ht -> gcc, key);
}


void rht_del (rht_t * ht, char * key)
{
  htab_remove_elt (ht -> gcc, key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  func_t fun = { .f = fn, .data = data };

  htab_traverse_noresize (ht -> gcc, foreach_cb, & fun);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  kvcb_t kv = { keys, NULL, 0 };
  htab_traverse_noresize (ht -> gcc, addkey_cb, & kv);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  kvcb_t kv = { NULL, vals, 0 };
  htab_traverse_noresize (ht -> gcc, addval_cb, & kv);
  return vals;
}
