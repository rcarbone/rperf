/* System headers */
#include <stdio.h>

/* The implementation */
#include "xxx.c"

/* librht - an abstract C library over real hash tables */
typedef xxx_t rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rht-hashers.h"


static unsigned hashfunc (char * key)
{
  return rht_python_hash (key);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  rht_t * ht = calloc (1, sizeof (* ht));
  return ht;
}


void rht_free (rht_t * ht)
{
  free (ht);
}


void rht_clear (rht_t * ht)
{
}


unsigned rht_count (rht_t * ht)
{
  return 0;
}


void rht_set (rht_t * ht, char * key, void * val)
{
}


void * rht_get (rht_t * ht, char * key)
{
  return NULL;
}


void rht_del (rht_t * ht, char * key)
{
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  unsigned n = rht_count (ht);
  unsigned i;
  for (i = 0; i < n; i ++)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  unsigned n = rht_count (ht);
  char ** keys = calloc (n + 1, sizeof (char *));
  unsigned i;
  for (i = 0; i < ht -> n; i ++)
    keys [i ++] = xxx;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  unsigned n = rht_count (ht);
  char ** vals = calloc (n + 1, sizeof (char *));
  unsigned i;
  for (i = 0; i < ht -> n; i ++)
    vals [i ++] = xxx;
  return vals;
}
