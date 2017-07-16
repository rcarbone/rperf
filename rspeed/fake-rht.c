/* System headers */
#include <stdio.h>
#include <stdlib.h>


/* librht - an abstract C library over real hash table implementations */
typedef struct rht rht_t;
#include "rht.h"


typedef struct rht
{
  unsigned count;

} rht_t;


static void never (char * func)
{
  printf ("The function [%s] is only for reference and it should never be called\n", func);
}


rht_t * rht_alloc (unsigned size)
{
  never ("rht_alloc");
  return (rht_t *) calloc (1, sizeof (rht_t));
}


void rht_free (rht_t * ht)
{
  never ("rht_free");
  if (ht)
    free (ht);
}


void rht_clear (rht_t * ht)
{
  never ("rht_clear");
}


unsigned rht_count (rht_t * ht)
{
  never ("rht_count");
  return ht ? ht -> count : 0;
}


void rht_set (rht_t * ht, char * key, void * val)
{
  never ("rht_set");
}


void * rht_get (rht_t * ht, char * key)
{
  never ("rht_get");
  return NULL;
}


void rht_del (rht_t * ht, char * key)
{
  never ("rht_del");
}


bool rht_has (rht_t * ht, char * key)
{
  never ("rht_has");
  return false;
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  never ("rht_foreach");
}


char ** rht_keys (rht_t * ht)
{
  never ("rht_keys");
  return NULL;
}


void ** rht_vals (rht_t * ht)
{
  never ("rht_vals");
  return NULL;
}
