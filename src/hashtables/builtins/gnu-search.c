/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* GNU libc */
#define __USE_GNU
#include <search.h>

typedef struct _ENTRY
{
  unsigned int used;
  ENTRY entry;
} _ENTRY;



/* librht - an abstract C library over real hash table implementations */
typedef struct hsearch_data rht_t;
#include "rht-api.h"


rht_t * rht_alloc (unsigned size)
{
  struct hsearch_data * ht = calloc (1, sizeof (rht_t));
  hcreate_r (size, ht);
  return ht;
}


void rht_free (rht_t * ht)
{
  hdestroy_r (ht);
  if (ht)
    free (ht);
}


void rht_clear (rht_t * ht)
{
  hdestroy_r (ht);
  ht -> filled = 0;
}


unsigned rht_count (rht_t * ht)
{
  return ht -> filled;
}


void rht_set (rht_t * ht, char * key, void * val)
{
  ENTRY e;
  ENTRY * ep;

  e . key  = key;
  e . data = val;

  hsearch_r (e, ENTER, & ep, ht);
}


void * rht_get (rht_t * ht, char * key)
{
  ENTRY e;
  ENTRY * ep = NULL;
  e . key = key;

  hsearch_r (e, FIND, & ep, ht);

  return ep ? ep -> data : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  ENTRY e;
  ENTRY * ep = NULL;
  e . key = key;

  hsearch_r (e, FIND, & ep, ht);
  if (ep)
    ht -> filled --;
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  unsigned i;
  for (i = 0; i < ht -> filled; i ++)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned j = 0;
  unsigned i;
  _ENTRY * table = ht -> table;
  /* prevent zero */
  for (i = 1; j < ht -> filled && i < ht -> size + 1; i ++)
    {
      ENTRY e = table [i] . entry;
      if (e . key && e . data)
	keys [j ++ ] = e . key;
    }
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned j = 0;
  unsigned i;
  _ENTRY * table = ht -> table;
  /* prevent zero */
  for (i = 1; j < ht -> filled && i < ht -> size + 1; i ++)
    {
      ENTRY e = table [i] . entry;
      if (e . key && e . data)
	vals [j ++ ] = e . data;
    }
  return vals;
}
