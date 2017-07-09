/* System headers */
#include <stdio.h>

/* The implementation */
#include "hashtab.c"

/* librht - an abstract C library over real hash tables */
typedef struct rht rht_t;
#include "rht.h"
#include "datasets.h"
#include "varrays.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

typedef struct rht
{
  htab_t gcc;
} rht_t;


typedef struct
{
  void (* f) (void *);
  void * data;

} func_t;


static hashval_t hash_str_fn (const void * key)
{
  return rht_python_hash (key);
}


static int eq_str_fn (const void * obj, const void * key)
{
  return ! strcmp (((robj_t *) obj) -> skey, key);
}


static int foreach (void ** slot, void * arg)
{
  func_t * func = arg;

  if (func && func -> f)
    func -> f (func -> data);

  return 1;
}


/* Callback to iterate over the hash table to add a key */
static int addkey (void ** slot, void * _keys)
{
  char ** keys = * (char ***) _keys;

  keys = (char **) vamore ((void **) keys, (* (robj_t **) slot) -> skey);
  * (char ***) _keys = keys;

  return 1;
}


/* Callback to iterate over the hash table to add a val */
static int addval (void ** slot, void * _vals)
{
  void ** vals = * (void ***) _vals;

  vals = vamore (vals, (* (robj_t **) slot) -> pval);
  * (void ***) _vals = vals;

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

  htab_traverse_noresize (ht -> gcc, foreach, & fun);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = NULL;
  htab_traverse_noresize (ht -> gcc, addkey, & keys);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = NULL;
  htab_traverse_noresize (ht -> gcc, addval, & vals);
  return vals;
}
