/* System headers */
#include <stdio.h>

/* The implementation */
#include "ht-internal.h"

/* librht - an abstract C library over real hash tables */
#define NEED_RHT_TYPEDEF
#include "rht.h"
#include "datasets.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


#define LOAD_LIMIT 0.9

/* Forward */
static unsigned hash_fn (robj_t * leobj);
static int cmp_fn (robj_t * a, robj_t * b);

/* Hash table naming and definition */
HT_HEAD(rht, robj);

/* Generates prototypes and inline functions */
HT_PROTOTYPE (rht, robj, _levt_, hash_fn, cmp_fn)
HT_GENERATE (rht, robj, _levt_, hash_fn, cmp_fn, LOAD_LIMIT, malloc, realloc, free)

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

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (* ht));
  HT_INIT (rht, ht);
  return ht;
}


void rht_free (rht_t * ht)
{
  HT_CLEAR (rht, ht);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  HT_CLEAR (rht, ht);
}


unsigned rht_count (rht_t * ht)
{
  return HT_SIZE (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  robj_t * obj = val;
  HT_INSERT (rht, ht, obj);
}


void * rht_get (rht_t * ht, char * key)
{
  robj_t obj = { .skey = key };
  robj_t * hit;

  hit = HT_FIND (rht, ht, & obj);

  return hit ? hit -> pval : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  robj_t obj = { .skey = key };
  robj_t * hit;

  hit = HT_FIND (rht, ht, & obj);
  if (hit)
    HT_REMOVE (rht, ht, hit);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  robj_t ** obj;
  HT_FOREACH (obj, rht, ht)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  robj_t ** k;
  HT_FOREACH (k, rht, ht)
    keys [i ++] = (* k) -> skey;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  robj_t ** k;
  HT_FOREACH (k, rht, ht)
    vals [i ++] = (* k) -> pval;
  return vals;
}
