/* System headers */
#include <stdio.h>

/* The implementation */
#include "pyhash.c"
#include "hashtable.c"

/* librhash - an abstract C library over real hash tables */
typedef _Py_hashtable_t rhash_t;
#include "rhash.h"
#include "varrays.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
#define PTR_VAL_SIZE sizeof (void *)

typedef struct
{
  void (* fn) (void *);
  void * data;

} func_t;


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
static int addkey (_Py_hashtable_entry_t * obj, void * keys)
{
  char ** k = * (char ***) keys;
  k = (char **) vamore ((void **) k, (char *) obj -> key);
  * (char ***) keys = k;
  return 0;
}


/* Callback to iterate over the hash table to add a val */
static int addval (_Py_hashtable_entry_t * obj, void * vals)
{
  void ** v = * (void ***) vals;
  v = vamore (v, _Py_HASHTABLE_ENTRY_DATA_AS_VOID_P (obj));
  * (void ***) vals = v;
  return 0;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  return _Py_hashtable_new (PTR_VAL_SIZE, _Py_hashtable_hash_str, _Py_hashtable_compare_string);
}


void rhash_free (rhash_t * ht)
{
  _Py_hashtable_destroy (ht);
}


void rhash_clear (rhash_t * ht)
{
  _Py_hashtable_clear (ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return _Py_hashtable_count (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  _Py_HASHTABLE_SET (ht, key, val);
}


void * rhash_get (rhash_t * ht, char * key)
{
  _Py_hashtable_entry_t * e = _Py_hashtable_get_entry (ht, key);
  return e ? _Py_HASHTABLE_ENTRY_DATA_AS_VOID_P (e) : NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  _Py_hashtable_delete (ht, key);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  _Py_hashtable_foreach (ht, myforeach, & fun);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  _Py_hashtable_foreach (ht, addkey, & keys);
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (void *));
  _Py_hashtable_foreach (ht, addval, & vals);
  return vals;
}
