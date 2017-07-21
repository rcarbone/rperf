/* System headers */
#include <stdio.h>

/* The implementation */
#include "pyhash.c"
#include "hashtable.c"

/* librhash - an abstract C library over real hash tables */
typedef _Py_hashtable_t rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
#define PTR_VAL_SIZE sizeof (void *)


/* Typedef used in the foreach callback */
typedef struct
{
  void (* fn) (void *);
  void * data;

} func_t;


/* Typedef used in the keys/vals callbacks */
typedef struct
{
  char ** keys;
  void ** vals;
  unsigned i;

} kvcb_t;



static unsigned _Py_hashtable_count (_Py_hashtable_t * ht)
{
  return ht ? ht -> entries : 0;
}


static Py_uhash_t _Py_hashtable_hash_str (const void * key)
{
  return rht_python_hash (key);
}


static int _Py_hashtable_compare_string (const void * key, const _Py_hashtable_entry_t * entry)
{
  return ! strcmp (entry -> key, key);
}


static int myforeach (_Py_hashtable_entry_t * obj, void * arg)
{
  func_t * func = arg;
  if (func)
    func -> fn (func -> data);
  return 0;
}


/* Callback to iterate over the hash table to add a key */
static int addkey (_Py_hashtable_entry_t * obj, void * arg)
{
  kvcb_t * kv = arg;
  kv -> keys [kv -> i ++] = (char *) obj -> key;
  return 0;
}


/* Callback to iterate over the hash table to add a val */
static int addval (_Py_hashtable_entry_t * obj, void * arg)
{
  kvcb_t * vals = arg;
  vals -> vals [vals -> i ++] = _Py_HASHTABLE_ENTRY_DATA_AS_VOID_P (obj);
  return 0;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  return _Py_hashtable_new (PTR_VAL_SIZE, _Py_hashtable_hash_str, _Py_hashtable_compare_string);
}


void rht_free (rht_t * ht)
{
  _Py_hashtable_destroy (ht);
}


void rht_clear (rht_t * ht)
{
  _Py_hashtable_clear (ht);
}


unsigned rht_count (rht_t * ht)
{
  return _Py_hashtable_count (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  _Py_HASHTABLE_SET (ht, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  _Py_hashtable_entry_t * e = _Py_hashtable_get_entry (ht, key);
  return e ? _Py_HASHTABLE_ENTRY_DATA_AS_VOID_P (e) : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  _Py_hashtable_delete (ht, key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  _Py_hashtable_foreach (ht, myforeach, & fun);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  kvcb_t kv = { keys, NULL, 0 };
  _Py_hashtable_foreach (ht, addkey, & kv);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  kvcb_t kv = { NULL, vals, 0 };
  _Py_hashtable_foreach (ht, addval, & kv);
  return vals;
}
