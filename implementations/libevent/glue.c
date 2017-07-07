/* System headers */
#include <stdio.h>

/* The implementation */
#include "ht-internal.h"

/* librhash - an abstract C library over real hash tables */
typedef struct rhash rhash_t;
#include "rhash.h"
#include "datasets.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


#define LOAD_LIMIT 0.9

/* Forward */
static unsigned hash_fn (robj_t * leobj);
static int cmp_fn (robj_t * a, robj_t * b);

/* Hash table naming and definition */
HT_HEAD(rhash, robj);

/* Generates prototypes and inline functions */
HT_PROTOTYPE (rhash, robj, _levt_, hash_fn, cmp_fn)
HT_GENERATE (rhash, robj, _levt_, hash_fn, cmp_fn, LOAD_LIMIT, malloc, realloc, free)

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Hash string keys using the internal helper function */
static unsigned hash_fn (robj_t * obj)
{
  return rht_python_hash (obj -> skey);
}


/* Compare 2 string keys for equality */
static int cmp_fn (robj_t * a, robj_t * b)
{
  return ! strcmp (a -> skey, b -> skey);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  rhash_t * ht = calloc (1, sizeof (rhash_t));
  HT_INIT (rhash, ht);
  return ht;
}


void rhash_free (rhash_t * ht)
{
  rhash_clear (ht);
  free (ht);
}


void rhash_clear (rhash_t * ht)
{
  HT_CLEAR (rhash, ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return HT_SIZE (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  robj_t * obj = val;
  HT_REPLACE (rhash, ht, obj);
}


void * rhash_get (rhash_t * ht, char * key)
{
  robj_t obj = { .skey = key };
  robj_t * hit;

  hit = HT_FIND (rhash, ht, & obj);

  return hit ? hit -> pval : NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  robj_t obj = { .skey = key };
  robj_t * hit;

  hit = HT_FIND (rhash, ht, & obj);
  if (hit)
    HT_REMOVE (rhash, ht, hit);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  robj_t ** obj;
  HT_FOREACH (obj, rhash, ht)
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  robj_t ** k;
  HT_FOREACH (k, rhash, ht)
    keys [i ++] = (* k) -> skey;
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  robj_t ** k;
  HT_FOREACH (k, rhash, ht)
    vals [i ++] = (* k) -> pval;
  return vals;
}
