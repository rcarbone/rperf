/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include "uthash.h"

typedef struct rhash rhash_t;
#include "rhash.h"
#include "varrays.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Private Object Type required to make it hash-able */
typedef struct utobj utobj_t;
typedef struct utobj
{
  char    * key;
  utobj_t * val;

  UT_hash_handle hh;     /* field must be named hh in order to use the convenience macros */

} utobj_t;


struct rhash
{
  utobj_t * uthash;
};


static utobj_t * mkpair (char * key, void * val)
{
  utobj_t * obj = calloc (1, sizeof (* obj));
  obj -> key = strdup (key);
  obj -> val = val;
  return obj;
}


static void rmpair (utobj_t * obj)
{
  free (obj -> key);
  free (obj);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  rhash_t * ht = calloc (1, sizeof (* ht));
  ht -> uthash = NULL;                       /* Important! initialize to NULL */
  return ht;
}


void rhash_free (rhash_t * ht)
{
  char ** keys = rhash_keys (ht);
  char ** k = keys;
  while (* k)
    rhash_del (ht, * k ++);
  if (keys)
    free (keys);
  HASH_CLEAR (hh, ht -> uthash);
  free (ht);
}


void rhash_clear (rhash_t * ht)
{
  char ** keys = rhash_keys (ht);
  char ** k = keys;
  while (* k)
    rhash_del (ht, * k ++);
  if (keys)
    free (keys);
  HASH_CLEAR (hh, ht -> uthash);
  ht -> uthash = NULL;                       /* Important! initialize to NULL */
}


unsigned rhash_count (rhash_t * ht)
{
  return HASH_COUNT (ht -> uthash);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  utobj_t * obj = mkpair (key, val);
  HASH_ADD_KEYPTR (hh, ht -> uthash, key, strlen (key), obj);
}


void * rhash_get (rhash_t * ht, char * key)
{
  utobj_t * hit;
  HASH_FIND_STR (ht -> uthash, key, hit);
  return hit ? hit -> val : NULL;
}


void rhash_del (rhash_t * ht, char * key)
{
  utobj_t * hit;
  HASH_FIND_STR (ht -> uthash, key, hit);
  if (hit)
    {
      HASH_DEL (ht -> uthash, hit);
      rmpair (hit);
    }
}


bool rhash_has (rhash_t * ht, char * key)
{
  return rhash_get (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  utobj_t * it;
  for (it = ht -> uthash; it; it = (utobj_t *) (it -> hh . next))
    fn (data);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  utobj_t * it;
  for (it = ht -> uthash; it; it = (utobj_t *) (it -> hh . next))
    keys = (char **) vamore ((void **) keys, it -> hh . key);
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (char *));
  utobj_t * it;
  for (it = ht -> uthash; it; it = (utobj_t *) (it -> hh . next))
    vals = vamore (vals, rhash_get (ht, it -> hh . key));
  return vals;
}

