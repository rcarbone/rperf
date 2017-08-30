/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include <Judy.h>

/* librht - an abstract C library over real hash tables */
#define NEED_RHT_TYPEDEF
#include "rht-api.h"
#include "datasets.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

typedef struct rht
{
  Pvoid_t judy;
  unsigned count;  /* needed due to a possible bug in JudySL - insert the value after JSLI changes the JLC */

} rht_t;



/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  return calloc (1, sizeof (rht_t));
}


void rht_free (rht_t * ht)
{
  Word_t bytes;
  JSLFA (bytes, ht -> judy);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  Word_t bytes;
  JSLFA (bytes, ht -> judy);
  ht -> count = 0;
}


unsigned rht_count (rht_t * ht)
{
#if defined(ROCCO)
  Word_t count = 0;
  JLC (count, ht -> judy, 0, -1);
  return count;
#else
  return ht -> count;
#endif /* ROCCO */
}


void rht_set (rht_t * ht, char * key, void * val)
{
  robj_t ** slot;
  JSLI (slot, ht -> judy, key);
  * slot = val;
  ht -> count ++;
}


void * rht_get (rht_t * ht, char * key)
{
  robj_t ** val;
  JSLG (val, ht -> judy, key);
  return val ? * val : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  robj_t ** val;
  JSLG (val, ht -> judy, key);
  if (val)
    {
      Word_t ret;
      JSLD (ret, ht -> judy, key);
      ht -> count --;
    }
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  char key [128] = { 0x0 };
  Pvoid_t val;
  JSLF (val, ht -> judy, key);
  while (val)
    {
      fn (data);
      JSLN (val, ht -> judy, key);
    }
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  char key [128] = { 0x0 };
  Pvoid_t val;
  JSLF (val, ht -> judy, key);
  while (val)
    {
      keys [i ++] = strdup (key);
      JSLN (val, ht -> judy, key);
    }
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  char key [128] = { 0x0 };
  Pvoid_t val;
  JSLF (val, ht -> judy, key);
  while (val)
    {
      vals [i ++] = val;
      JSLN (val, ht -> judy, key);
    }
  return vals;
}
