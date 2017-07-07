/* System headers */
#include <stdio.h>

/* The implementation */
#include "tommyhashlin.c"

/* librhash - an abstract C library over real hash tables */
typedef tommy_hashlin rhash_t;
#include "rhash.h"
#include "datasets.h"
#include "varrays.c"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

typedef struct
{
  char * key;
  robj_t * found;

} look_t;


typedef struct
{
  rhash_each_f * fn;
  void * data;

} func_t;


/* The callback comparison function to look up for matching keys */
static int cmp_str (const void * key, const void * obj)
{
  return ! strcmp (key, ((robj_t *) obj) -> skey);
}


/* Callback to iterate over the hash table to search for a key */
static bool searchkey (void * arg, void * obj)
{
  look_t * l = arg;
  if (l && l -> key && ! strcmp (l -> key, ((robj_t *) obj) -> skey))
    {
      l -> found = obj;
      return true;
    }
  return false;
}


/* Callback to iterate over the hash table to search for a key */
static bool myforeach (void * arg, void * obj)
{
  func_t * func = arg;
  if (func)
    func -> fn (func -> data);
  return false;
}


/* Callback to iterate over the hash table to add a key */
static bool addkey (void * keys, void * obj)
{
  char ** k = * (char ***) keys;
  k = (char **) vamore ((void **) k, ((robj_t *) obj) -> skey);
  * (char ***) keys = k;
  return false;
}


/* Callback to iterate over the hash table to add a val */
static bool addval (void * vals, void * obj)
{
  void ** v = * (void ***) vals;
  v = vamore (v, ((robj_t *) obj) -> pval);
  * (void ***) vals = v;
  return false;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  rhash_t * ht = calloc (1, sizeof (* ht));
  tommy_hashlin_init (ht);
  return ht;
}


void rhash_free (rhash_t * ht)
{
  if (tommy_hashlin_count (ht))
    tommy_hashlin_done (ht);
  free (ht);
}


void rhash_clear (rhash_t * ht)
{
  if (tommy_hashlin_count (ht))
    tommy_hashlin_done (ht);
  ht -> count = 0;                   /* Bug! */
}


unsigned rhash_count (rhash_t * ht)
{
  return tommy_hashlin_count (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  robj_t * obj = val;
  tommy_hashlin_insert (ht, & obj -> tommy, obj, rht_python_hash (key));
}


void * rhash_get (rhash_t * ht, char * key)
{
  /* Set input/output parameters to the iterate callback */
  look_t l = { .key = key, .found = NULL };
  tommy_hashlin_foreach_arg (ht, searchkey, & l);
  return l . found;
}


void rhash_del (rhash_t * ht, char * key)
{
  robj_t * obj = rhash_get (ht, key);
  if (obj)
    tommy_hashlin_remove (ht, cmp_str, obj, rht_python_hash (key));
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  tommy_hashlin_foreach_arg (ht, myforeach, & fun);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = NULL;
  tommy_hashlin_foreach_arg (ht, addkey, & keys);
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = NULL;
  tommy_hashlin_foreach_arg (ht, addval, & vals);
  return vals;
}
