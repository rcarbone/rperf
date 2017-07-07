/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The implementation */
#include "ht-internal.h"

/* librhash - an abstract C library over real hash tables */
typedef struct leht rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#define LOAD_LIMIT 0.9

/* Types (hash table and objects) */
typedef struct leobj leobj_t;

/* Forward */
static unsigned le_hash_f (leobj_t * leobj);
static int le_cmp_f (leobj_t * a, leobj_t * b);

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Object definition */
struct leobj
{
  char * key;
  leobj_t * val;

  HT_ENTRY(leobj) _reserved_;   /* field required to make the object hash-able */
};

/* Hash table naming and definition */
HT_HEAD(leht, leobj);

/* Generates prototypes and inline functions */
HT_PROTOTYPE (leht, leobj, _reserved_, le_hash_f, le_cmp_f)
HT_GENERATE (leht, leobj, _reserved_, le_hash_f, le_cmp_f, LOAD_LIMIT, malloc, realloc, free)


/* Hash string keys using the internal helper function */
static unsigned le_hash_f (leobj_t * obj)
{
  return ht_string_hash_ (obj -> key);
}


/* Compare 2 string keys for equality */
static int le_cmp_f (leobj_t * a, leobj_t * b)
{
  return ! strcmp (a -> key, b -> key);
}


static leobj_t * mk (char * key, void * val)
{
  leobj_t * obj = calloc (1, sizeof (* obj));
  obj -> key = strdup (key);
  obj -> val = val;
  return obj;
}


static void rm (leobj_t * obj)
{
  free (obj -> key);
  free (obj);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  rhash_t * ht = calloc (1, sizeof (rhash_t));
  HT_INIT (leht, ht);
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
  leobj_t ** k;

  HT_FOREACH (k, leht, ht)
    objs [i ++] = * k;

  k = objs;
  while (* k)
    rm (* k ++);

  free (objs);

  HT_CLEAR (leht, ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return HT_SIZE (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  HT_REPLACE (leht, ht, mk (key, val));
}


void * rhash_get (rhash_t * ht, char * key)
{
  leobj_t obj = { .key = key };
  leobj_t * hit;

  hit = HT_FIND (leht, ht, & obj);

  return hit ? hit -> val : NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  leobj_t obj = { .key = key };
  leobj_t * hit;

  hit = HT_FIND (leht, ht, & obj);
  if (hit)
    {
      HT_REMOVE (leht, ht, hit);
      rm (hit);
    }
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, void (* fn) (void * x), void * data)
{
  leobj_t ** k;
  HT_FOREACH (k, leht, ht)
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  leobj_t ** k;
  HT_FOREACH (k, leht, ht)
    keys [i ++] = (* k) -> key;
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  leobj_t ** k;
  HT_FOREACH (k, leht, ht)
    vals [i ++] = (* k) -> val;
  return vals;
}
