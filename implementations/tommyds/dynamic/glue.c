/* System headers */
#include <stdio.h>

/* The implementation */
#include "tommyhashdyn.c"

/* librhash - an abstract C library over real hash tables */
typedef tommy_hashdyn rht_t;
#include "rht.h"
#include "datasets.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* Typedef used in the foreach callback */
typedef struct
{
  rht_each_f * fn;
  void * data;

} func_t;


/* Typedef used in the keys/vals callbacks */
typedef struct
{
  char ** keys;
  void ** vals;
  unsigned i;

} kvcb_t;


/* The callback comparison function to look up for matching keys */
static int cmp_str (const void * key, const void * obj)
{
  return ! strcmp (key, ((robj_t *) obj) -> skey);
}


/* Callback to iterate over the hash table to search for a key */
static bool foreach_cb (void * arg, void * obj)
{
  func_t * func = arg;
  if (func)
    func -> fn (func -> data);
  return false;
}


/* Callback to iterate over the hash table to add a key */
static bool addkey_cb (void * arg, void * data)
{
  kvcb_t * kv = arg;
  tommy_hashdyn_node * node = data;
#if defined(ROCCO)
  // kv -> keys [kv -> i ++] = node -> key;
#else
  kv -> keys [kv -> i ++] = "rocco";
  // printf ("i = %u - key = [%p]\n", kv -> i, ((tommy_hashdyn_node *) data) -> key);
#endif /* ROCCO */
  return false;
}


/* Callback to iterate over the hash table to add a val */
static bool addval_cb (void * arg, void * data)
{
  kvcb_t * kv = arg;
  tommy_hashdyn_node * node = data;
  kv -> vals [kv -> i ++] = node -> data;
  return false;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (* ht));
  tommy_hashdyn_init (ht);
  return ht;
}


void rht_free (rht_t * ht)
{
  if (tommy_hashdyn_count (ht))
    tommy_hashdyn_done (ht);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  if (tommy_hashdyn_count (ht))
    tommy_hashdyn_done (ht);
  ht -> count = 0;                   /* Bug! */
}


unsigned rht_count (rht_t * ht)
{
  return tommy_hashdyn_count (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  robj_t * obj = val;
  tommy_hashdyn_insert (ht, & obj -> tommy, obj, rht_python_hash (key));
}


void * rht_get (rht_t * ht, char * key)
{
  robj_t item;
  return tommy_hashdyn_search (ht, cmp_str, & item, rht_python_hash (key));
}


void rht_del (rht_t * ht, char * key)
{
  robj_t * obj = rht_get (ht, key);
  if (obj)
    tommy_hashdyn_remove (ht, cmp_str, obj, rht_python_hash (key));
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  tommy_hashdyn_foreach_arg (ht, foreach_cb, & fun);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  kvcb_t kv = { keys, NULL, 0 };
  tommy_hashdyn_foreach_arg (ht, addkey_cb, & kv);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  kvcb_t kv = { NULL, vals, 0 };
  tommy_hashdyn_foreach_arg (ht, addval_cb, & kv);
  return vals;
}
