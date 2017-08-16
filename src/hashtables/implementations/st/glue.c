/* System headers */
#include <stdio.h>

/* The implementation */
#include "st.c"

/* librht - an abstract C library over real hash tables */
typedef st_table rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

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


static unsigned st_count (rht_t * ht)
{
  return ht -> num_entries;
}


static int clear_cb (char * key, void * val, void * arg)
{
  rht_t * ht = arg;
  st_data_t safe = { 0 };
  st_delete_safe (ht, (st_data_t *) & key, NULL, safe);
  return 0;
}


static int foreach_cb (char * key, void * val, void * arg)
{
  func_t * func = arg;
  func -> f (func -> data);
  return 0;
}


/* Callback to iterate over the hash table to add a key */
static int addkey_cb (char * key, void * val, void * arg)
{
  kvcb_t * kv = arg;
  kv -> keys [kv -> i ++] = key;
  return 0;
}


/* Callback to iterate over the hash table to add a key */
static int addval_cb (char * key, void * val, void * arg)
{
  kvcb_t * kv = arg;
  kv -> vals [kv -> i ++] = val;
  return 0;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  return st_init_strtable ();
}


void rht_free (rht_t * ht)
{
  st_free_table (ht);
}


void rht_clear (rht_t * ht)
{
  st_foreach (ht, clear_cb, (st_data_t) ht);
}


unsigned rht_count (rht_t * ht)
{
  return st_count (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  st_insert (ht, (st_data_t) key, (st_data_t) val);
}


void * rht_get (rht_t * ht, char * key)
{
  st_data_t hit;
  return st_lookup (ht, (st_data_t) key, & hit) ? (void *) hit : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  st_delete (ht, (st_data_t *) & key, NULL);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  func_t fun = { .f = fn, .data = data };
  st_foreach (ht, foreach_cb, (st_data_t) & fun);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  kvcb_t kv = { keys, NULL, 0 };
  st_foreach (ht, addkey_cb, (st_data_t) & kv);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  kvcb_t kv = { NULL, vals, 0 };
  st_foreach (ht, addval_cb, (st_data_t) & kv);
  return vals;
}
