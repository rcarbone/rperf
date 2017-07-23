/* System headers */
#include <stdio.h>

/* The implementation */
#define STB_DEFINE
#include "stb.h"


/* librht - an abstract C library over real hash tables */
typedef stb_sdict rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static unsigned int hashfunc (const char * key)
{
  return rht_python_hash (key);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  return stb_sdict_new (0);
}


void rht_free (rht_t * ht)
{
  stb_sdict_destroy (ht);
}


void rht_clear (rht_t * ht)
{
  unsigned i;
  char * key;
  void * val;
  stb_sdict_for (ht, i, key, val)
    stb_sdict_remove (ht, key, & val);
  ht -> count = 0;
}


unsigned rht_count (rht_t * ht)
{
  return stb_sdict_count (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  stb_sdict_set (ht, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  return stb_sdict_get (ht, key);
}


void rht_del (rht_t * ht, char * key)
{
  void * hit;
  stb_sdict_remove (ht, key, & hit);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  unsigned i;
  char * key;
  void * val;
  stb_sdict_for (ht, i, key, val)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  unsigned j;
  char * key;
  void * val;
  stb_sdict_for (ht, j, key, val)
    keys [i ++] = key;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  unsigned j;
  char * key;
  void * val;
  stb_sdict_for (ht, j, key, val)
    vals [i ++] = key;
  return vals;
}
