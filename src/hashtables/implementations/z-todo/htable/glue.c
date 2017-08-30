/* System headers */
#include <stdio.h>

/* The implementation */
#include "htable.h"

/* Project headers */
#include "datasets.h"

/* librht - an abstract C library over real hash tables */
typedef struct hash_table rht_t;
#include "rht-api.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static unsigned int hashfunc (const char * key)
{
  return rht_python_hash (key);
}


static int hash_table_count (struct hash_table * ht)
{
  return ht -> num_elems;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  return calloc (1, sizeof (rht_t));
}


void rht_free (rht_t * ht)
{
  free (ht);
}


void rht_clear (rht_t * ht)
{
#if defined(ROCCO)
  apr_hash_clear (ht);
#endif /* ROCCO */
}


unsigned rht_count (rht_t * ht)
{
  return hash_table_count (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  robj_t * obj = val;
  hash_table_insert (ht, & obj -> _htable_, key, strlen (key));
}


void * rht_get (rht_t * ht, char * key)
{
  struct hash_entry * hit = hash_table_lookup_key (ht, key, strlen (key));
  if (hit)
    {
      return hit;
      // robj_t * obj = (robj_t *) hit;
      // void * val = hash_entry (hit, void *, & obj -> _htable_);
      // return hash_entry (hit, robj_t, & obj -> _htable_) -> pval;
    }
  else
    return NULL;
}


void rht_del (rht_t * ht, char * key)
{
#if defined(ROCCO)
  apr_hash_del (ht, key, strlen (key));
#endif /* ROCCO */
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
#if defined(ROCCO)
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    fn (data);
#endif /* ROCCO */
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
#if defined(ROCCO)
  unsigned i = 0;
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    keys [i ++] = (char *) apr_hash_this_key (k);
#endif /* ROCCO */
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
#if defined(ROCCO)
  unsigned i = 0;
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    vals [i ++] = (void *) apr_hash_this_val (k);
#endif /* ROCCO */
  return vals;
}
