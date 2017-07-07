/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The implementation */
#include "ht-internal.h"

/* librhash - an abstract C library over real hash tables */
typedef struct rhash rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#define LOAD_LIMIT 0.9

/* Private Object Type required to make it hash-able */
typedef struct leobj leobj_t;
struct leobj
{
  char    * key;
  leobj_t * val;

  HT_ENTRY(leobj) _levt_;   /* field required to make the object hash-able */

};


/* Forward */
static unsigned hash_fn (leobj_t * leobj);
static int cmp_fn (leobj_t * a, leobj_t * b);

/* Hash table naming and definition */
HT_HEAD(rhash, leobj);

/* Generates prototypes and inline functions */
HT_PROTOTYPE (rhash, leobj, _levt_, hash_fn, cmp_fn)
HT_GENERATE (rhash, leobj, _levt_, hash_fn, cmp_fn, LOAD_LIMIT, malloc, realloc, free)

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Hash string keys using the internal helper function */
static unsigned hash_fn (leobj_t * obj)
{
  return rht_python_hash (obj -> key);
}


/* Compare 2 string keys for equality */
static int cmp_fn (leobj_t * a, leobj_t * b)
{
  return ! strcmp (a -> key, b -> key);
}


static leobj_t * mkpair (char * key, void * val)
{
  leobj_t * obj = calloc (1, sizeof (* obj));
  obj -> key = strdup (key);
  obj -> val = val;
  return obj;
}


static void rmpair (leobj_t * obj)
{
  free (obj -> key);
  free (obj);
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
  leobj_t ** objs = calloc (rhash_count (ht) + 1, sizeof (leobj_t *));
  unsigned i = 0;
  leobj_t ** obj;

  HT_FOREACH (obj, rhash, ht)
    objs [i ++] = * obj;

  obj = objs;
  while (* obj)
    rmpair (* obj ++);

  free (objs);

  HT_CLEAR (rhash, ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return HT_SIZE (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  HT_REPLACE (rhash, ht, mkpair (key, val));
}


void * rhash_get (rhash_t * ht, char * key)
{
  leobj_t obj = { .key = key };
  leobj_t * hit;

  hit = HT_FIND (rhash, ht, & obj);

  return hit ? hit -> val : NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  leobj_t obj = { .key = key };
  leobj_t * hit;

  hit = HT_FIND (rhash, ht, & obj);
  if (hit)
    {
      HT_REMOVE (rhash, ht, hit);
      rmpair (hit);
    }
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  leobj_t ** obj;
  HT_FOREACH (obj, rhash, ht)
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  leobj_t ** k;
  HT_FOREACH (k, rhash, ht)
    keys [i ++] = (* k) -> key;
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  leobj_t ** k;
  HT_FOREACH (k, rhash, ht)
    vals [i ++] = (* k) -> val;
  return vals;
}
