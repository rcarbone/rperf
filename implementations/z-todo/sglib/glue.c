/* System headers */
#include <stdio.h>

/* Project headers */
#include "datasets.h"

/* The implementation */
#include "sglib.h"


/* define the initial size of the hash table (0 is illegal) */
#define HASHSIZE  100000

/* Forward */
static uint32_t hash_obj (robj_t * obj);
static int cmp_objs (robj_t * a, robj_t * b);

/* Generates typedef and inline functions for the base container (usually a linked list) */
SGLIB_DEFINE_LIST_PROTOTYPES (robj_t, cmp_objs, _sglib_)
SGLIB_DEFINE_LIST_FUNCTIONS (robj_t, cmp_objs, _sglib_)

/* Generates typedef and inline functions for hash table */
SGLIB_DEFINE_HASHED_CONTAINER_PROTOTYPES (robj_t, HASHSIZE, hash_obj)
SGLIB_DEFINE_HASHED_CONTAINER_FUNCTIONS (robj_t, HASHSIZE, hash_obj)


/* librht - an abstract C library over real hash tables */
typedef robj_t rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Define a hash function for both string keys */
static uint32_t hash_obj (robj_t * obj)
{
  return rht_python_hash (obj -> skey);
}


/* Define a compare function for both string keys */
static int cmp_objs (robj_t * a, robj_t * b)
{
  return strcmp (a -> skey, b -> skey);
}


/* Iterate over all the objects to evaluate the hash table size */
static unsigned sglib_hashed_robj_t_count (rht_t ** ht)
{
  unsigned size = 0;
  robj_t * obj;
  struct sglib_hashed_robj_t_iterator it;

  for (obj = sglib_hashed_robj_t_it_init (& it, ht); obj; obj = sglib_hashed_robj_t_it_next (& it))
    size ++;

  return size;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  robj_t ** ht = calloc (HASHSIZE, sizeof (* ht));
  sglib_hashed_robj_t_init (ht);
  return  ht;
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
  return sglib_hashed_robj_t_count (& ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  sglib_hashed_robj_t_add (& ht, val);
}


void * rht_get (rht_t * ht, char * key)
{
  // return sglib_hashed_robj_t_find_member (& ht, key);
}


void rht_del (rht_t * ht, char * key)
{
  // robj_t * hit;
  // sglib_hashed_robj_t_delete_if_member (& ht, key, & hit);
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
