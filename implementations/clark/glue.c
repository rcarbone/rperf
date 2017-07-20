/* System headers */
#include <stdio.h>
#include <string.h>

/* Project headers */
#include "varrays.h"
#include "args.h"

/* The implementation */
#include "hashtable.c"

/* librhash - an abstract C library over real hash tables */
typedef struct hashtable rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

#include "rht-hashers.h"


static unsigned hash_str_key_fn (void * key)
{
  return rht_python_hash (key);
}


static int eq_str_key_fn (void * key1, void * key2)
{
  return ! strcmp (key1, key2);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  return create_hashtable (0, hash_str_key_fn, eq_str_key_fn);
}


void rht_free (rht_t * ht)
{
  hashtable_destroy (ht, 0);
}


void rht_clear (rht_t * ht)
{
  struct entry ** table = ht -> table;
  unsigned i;
  for (i = 0; i < ht -> tablelength; i ++)
    {
      struct entry * e = table [i];
      while (e)
	{
	  struct entry * f = e;
	  e = e -> next;
	  free (f);
	}
    }
  ht -> tablelength = 0;
  ht -> entrycount = 0;
}


unsigned rht_count (rht_t * ht)
{
  return hashtable_count (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  hashtable_insert (ht, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  return hashtable_search (ht, key);
}


void rht_del (rht_t * ht, char * key)
{
  hashtable_remove (ht, key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  struct entry ** table = ht -> table;
  unsigned i;
  for (i = 0; i < ht -> tablelength; i ++)
    {
      struct entry * e = table [i];
      while (e)
	{
	  fn (data);
	  e = e -> next;
	}
    }
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  struct entry ** table = ht -> table;
  unsigned k = 0;
  unsigned i;
  for (i = 0; i < ht -> tablelength; i ++)
    {
      struct entry * e = table [i];
      while (e)
	{
	  keys [k ++] = e -> k;
	  e = e -> next;
	}
    }
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  struct entry ** table = ht -> table;
  unsigned v = 0;
  unsigned i;
  for (i = 0; i < ht -> tablelength; i ++)
    {
      struct entry * e = table [i];
      while (e)
	{
	  vals [v ++] = e -> v;
	  e = e -> next;
	}
    }
  return vals;
}
