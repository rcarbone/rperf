/* System headers */
#include <stdlib.h>

/* GNU libc */
#include <search.h>


/* librht - an abstract C library over real hash table implementations */
typedef struct rht rht_t;
#include "rht.h"


typedef struct rht
{
  unsigned count;

} rht_t;


rht_t * rht_alloc (unsigned size)
{
  hcreate (size);
  return (rht_t *) calloc (1, sizeof (rht_t));
}


void rht_free (rht_t * ht)
{
  hdestroy ();
  if (ht)
    free (ht);
}


void rht_clear (rht_t * ht)
{
  hdestroy ();
  ht -> count = 0;
}


unsigned rht_count (rht_t * ht)
{
  return ht -> count;
}


void rht_set (rht_t * ht, char * key, void * val)
{
  ENTRY e;

  e . key  = key;
  e . data = val;

  hsearch (e, ENTER);
  ht -> count ++;
}


void * rht_get (rht_t * ht, char * key)
{
  ENTRY e;
  ENTRY * ep;
  e . key = key;

  ep = hsearch (e, FIND);

  return ep -> data;
}


void rht_del (rht_t * ht, char * key)
{
  ENTRY e;
  ENTRY * ep;
  e . key = key;

  ep = hsearch (e, FIND);
  if (ep)
    ht -> count --;
}


bool rht_has (rht_t * ht, char * key)
{
  return false;
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
}


char ** rht_keys (rht_t * ht)
{
  return NULL;
}


void ** rht_vals (rht_t * ht)
{
  return NULL;
}
