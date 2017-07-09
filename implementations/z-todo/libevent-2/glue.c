/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* The implementation */
#include "ht-internal.h"

/* librht - an abstract C library over real hash tables */
typedef struct rht rht_t;
#include "rht.h"

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
HT_HEAD(rht, leobj);

/* Generates prototypes and inline functions */
HT_PROTOTYPE (rht, leobj, _levt_, hash_fn, cmp_fn)
HT_GENERATE (rht, leobj, _levt_, hash_fn, cmp_fn, LOAD_LIMIT, malloc, realloc, free)

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

rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (rht_t));
  HT_INIT (rht, ht);
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
  leobj_t ** obj;

  HT_FOREACH (obj, rht, ht)
    objs [i ++] = * obj;

  obj = objs;
  while (* obj)
    rmpair (* obj ++);

  free (objs);

  HT_CLEAR (rht, ht);
}


unsigned rht_count (rht_t * ht)
{
  return HT_SIZE (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  HT_REPLACE (rht, ht, mkpair (key, val));
}


void * rht_get (rht_t * ht, char * key)
{
  leobj_t obj = { .key = key };
  leobj_t * hit;

  hit = HT_FIND (rht, ht, & obj);

  return hit ? hit -> val : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  leobj_t obj = { .key = key };
  leobj_t * hit;

  hit = HT_FIND (rht, ht, & obj);
  if (hit)
    {
      HT_REMOVE (rht, ht, hit);
      rmpair (hit);
    }
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  leobj_t ** obj;
  HT_FOREACH (obj, rht, ht)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  leobj_t ** k;
  HT_FOREACH (k, rht, ht)
    keys [i ++] = (* k) -> key;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  leobj_t ** k;
  HT_FOREACH (k, rht, ht)
    vals [i ++] = (* k) -> val;
  return vals;
}
