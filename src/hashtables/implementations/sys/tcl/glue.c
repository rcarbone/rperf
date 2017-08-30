/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include <tcl.h>

/* librhash - an abstract C library over real hash tables */
typedef Tcl_HashTable rht_t;
#include "rht-api.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  Tcl_HashTable * ht = calloc (1, sizeof (* ht));
  Tcl_InitHashTable (ht, TCL_STRING_KEYS);
  return ht;
}


void rht_free (rht_t * ht)
{
  Tcl_DeleteHashTable (ht);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  Tcl_HashSearch search;
  Tcl_HashEntry * he;
  for (he = Tcl_FirstHashEntry (ht, & search); he; he = Tcl_NextHashEntry (& search))
    Tcl_DeleteHashEntry (he);
}


unsigned rht_count (rht_t * ht)
{
  return ht -> numEntries;
}


void rht_set (rht_t * ht, char * key, void * val)
{
  int new_val;
  Tcl_HashEntry * he = Tcl_CreateHashEntry (ht, (const char *) key, & new_val);
  Tcl_SetHashValue (he, (ClientData) val);
}


void * rht_get (rht_t * ht, char * key)
{
  Tcl_HashEntry * he = Tcl_FindHashEntry (ht, (const char *) key);
  return he ? Tcl_GetHashValue (he) : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  Tcl_HashEntry * he = Tcl_FindHashEntry (ht, (const char *) key);
  if (he)
    Tcl_DeleteHashEntry (he);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  Tcl_HashSearch search;
  Tcl_HashEntry * he;
  for (he = Tcl_FirstHashEntry (ht, & search); he; he = Tcl_NextHashEntry (& search))
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  Tcl_HashSearch search;
  Tcl_HashEntry * he;
  for (he = Tcl_FirstHashEntry (ht, & search); he; he = Tcl_NextHashEntry (& search))
    keys [i ++] = Tcl_GetHashKey (ht, he);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  Tcl_HashSearch search;
  Tcl_HashEntry * he;
  for (he = Tcl_FirstHashEntry (ht, & search); he; he = Tcl_NextHashEntry (& search))
    vals [i ++] = Tcl_GetHashValue (he);
  return vals;
}
