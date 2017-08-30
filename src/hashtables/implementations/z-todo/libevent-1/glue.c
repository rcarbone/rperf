/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The implementation */
#include "ht-internal.h"

/* librht - an abstract C library over real hash tables */
typedef struct leht rht_t;
#include "rht-api.h"

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

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (rht_t));
  HT_INIT (leht, ht);
  return ht;
}


void rht_free (rht_t * ht)
{
  rht_clear (ht);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  leobj_t ** objs = calloc (rht_count (ht) + 1, sizeof (leobj_t *));
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


unsigned rht_count (rht_t * ht)
{
  return HT_SIZE (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  HT_REPLACE (leht, ht, mk (key, val));
}


void * rht_get (rht_t * ht, char * key)
{
  leobj_t obj = { .key = key };
  leobj_t * hit;

  hit = HT_FIND (leht, ht, & obj);

  return hit ? hit -> val : NULL;
}


void rht_del (rht_t * ht, char * key)
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


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, void (* fn) (void * x), void * data)
{
  leobj_t ** k;
  HT_FOREACH (k, leht, ht)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  leobj_t ** k;
  HT_FOREACH (k, leht, ht)
    keys [i ++] = (* k) -> key;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  leobj_t ** k;
  HT_FOREACH (k, leht, ht)
    vals [i ++] = (* k) -> val;
  return vals;
}
