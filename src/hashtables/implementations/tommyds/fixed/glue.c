/* System headers */
#include <stdio.h>

/* The implementation */
#include "tommyhashtbl.c"

/* librhash - an abstract C library over real hash tables */
typedef tommy_hashtable rht_t;
#include "rht-api.h"
#include "datasets.h"
#include "safe.h"


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
static void foreach_cb (void * arg, void * obj)
{
  func_t * func = arg;
  if (func)
    func -> fn (func -> data);
}


/* Callback to iterate over the hash table to add a key */
static void addkey_cb (void * arg, void * data)
{
  kvcb_t * kv = arg;
  robj_t * obj = data;
  kv -> keys [kv -> i ++] = obj -> skey;
}


/* Callback to iterate over the hash table to add a val */
static void addval_cb (void * arg, void * data)
{
  kvcb_t * kv = arg;
  kv -> vals [kv -> i ++] = data;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (* ht));
  tommy_hashtable_init (ht, size);
  return ht;
}


void rht_free (rht_t * ht)
{
  tommy_hashtable_done (ht);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  char ** keys = rht_keys (ht);
  char ** k = keys;
  while (k && * k)
    {
      robj_t * obj = rht_get (ht, * k);
      tommy_hashtable_remove (ht, cmp_str, obj, rht_python_hash (* k));
      k ++;
    }
  safefree (keys);
}


unsigned rht_count (rht_t * ht)
{
  return tommy_hashtable_count (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  robj_t * obj = val;
  tommy_hashtable_insert (ht, & obj -> _tommy_, obj, rht_python_hash (key));
}


void * rht_get (rht_t * ht, char * key)
{
  robj_t item = { .skey = key };
  return tommy_hashtable_search (ht, cmp_str, & item, rht_python_hash (key));
}


void rht_del (rht_t * ht, char * key)
{
  robj_t * obj = rht_get (ht, key);
  if (obj)
    tommy_hashtable_remove (ht, cmp_str, obj, rht_python_hash (key));
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  tommy_hashtable_foreach_arg (ht, foreach_cb, & fun);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  kvcb_t kv = { keys, NULL, 0 };
  tommy_hashtable_foreach_arg (ht, addkey_cb, & kv);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  kvcb_t kv = { NULL, vals, 0 };
  tommy_hashtable_foreach_arg (ht, addval_cb, & kv);
  return vals;
}
