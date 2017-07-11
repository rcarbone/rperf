/* System headers */
#include <stdio.h>
#include <string.h>

/* The implementation */
#include "tommyhashlin.c"

/* librhash - an abstract C library over real hash tables */
typedef tommy_hashlin rht_t;
#include "rht.h"
#include "varrays.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Private Object Type required to make it hash-able */
typedef struct tobj tobj_t;
struct tobj
{
  char * key;
  void * val;

  tommy_node tommy;
};


typedef struct
{
  char * key;
  tobj_t * found;

} look_t;


typedef struct
{
  rht_each_f * fn;
  void * data;

} func_t;


static tobj_t * mkpair (char * key, void * val)
{
  tobj_t * obj = calloc (1, sizeof (* obj));
  obj -> key = strdup (key);
  obj -> val = val;
  return obj;
}


static bool rmpair (void * obj)
{
  free (((tobj_t *) obj) -> key);
  free (obj);
  return false;
}


/* Callback to iterate over the hash table to search for a key */
static bool searchkey (void * arg, void * obj)
{
  look_t * l = arg;
  if (l && l -> key && ! strcmp (l -> key, ((tobj_t *) obj) -> key))
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
  k = (char **) vamore ((void **) k, ((tobj_t *) obj) -> key);
  * (char ***) keys = k;
  return false;
}


/* Callback to iterate over the hash table to add a val */
static bool addval (void * vals, void * obj)
{
  void ** v = * (void ***) vals;
  v = vamore (v, ((tobj_t *) obj) -> val);
  * (void ***) vals = v;
  return false;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (* ht));
  tommy_hashlin_init (ht);
  return ht;
}


void rht_free (rht_t * ht)
{
  if (tommy_hashlin_count (ht))
    tommy_hashlin_foreach (ht, rmpair);
  tommy_hashlin_done (ht);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  tommy_hashlin_foreach (ht, rmpair);
  ht -> count = 0;                   /* Bug! */
}


unsigned rht_count (rht_t * ht)
{
  return tommy_hashlin_count (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  tobj_t * obj = mkpair (key, val);
  tommy_hashlin_insert (ht, & obj -> tommy, obj, rht_python_hash (key));
}


void * rht_get (rht_t * ht, char * key)
{
  /* Set input/output parameters to the iterate callback */
  look_t l = { .key = key, .found = NULL };
  tommy_hashlin_foreach_arg (ht, searchkey, & l);
  return l . found ? l . found -> val : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  /* Set input/output parameters to the iterate callback */
  look_t l = { .key = key, .found = NULL };
  tommy_hashlin_foreach_arg (ht, searchkey, & l);
  if (l . found)
    {
      tommy_hashlin_remove_existing (ht, & l . found -> tommy);
      rmpair (l . found);
    }
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  tommy_hashlin_foreach_arg (ht, myforeach, & fun);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = NULL;
  tommy_hashlin_foreach_arg (ht, addkey, & keys);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = NULL;
  tommy_hashlin_foreach_arg (ht, addval, & vals);
  return vals;
}