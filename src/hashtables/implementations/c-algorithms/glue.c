/* System headers */
#include <stdio.h>

/* The implementation */
#include "hash-table.c"
#include "hash-string.c"
#include "compare-string.c"

/* librht - an abstract C library over real hash tables */
typedef HashTable rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static unsigned int strhash (void * key)
{
  return rht_python_hash (key);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  return hash_table_new (strhash, string_equal);
}


void rht_free (rht_t * ht)
{
  hash_table_free (ht);
}


void rht_clear (rht_t * ht)
{
  HashTableIterator k;
  hash_table_iterate (ht, & k);
  while (hash_table_iter_has_more (& k))
    {
      HashTablePair pair = hash_table_iter_next (& k);
      hash_table_remove (ht, pair . key);
    }
}


unsigned rht_count (rht_t * ht)
{
  return hash_table_num_entries (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  hash_table_insert (ht, key, val);
}


void * rht_get (rht_t * ht, char * key)
{
  return hash_table_lookup (ht, key);
}


void rht_del (rht_t * ht, char * key)
{
  hash_table_remove (ht, key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  HashTableIterator k;
  hash_table_iterate (ht, & k);
  while (hash_table_iter_has_more (& k))
    {
      fn (data);
      hash_table_iter_next (& k);
    }
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  HashTableIterator k;
  hash_table_iterate (ht, & k);
  while (hash_table_iter_has_more (& k))
    {
      HashTablePair pair = hash_table_iter_next (& k);
      keys [i ++] = pair . key;
    }
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  HashTableIterator k;
  hash_table_iterate (ht, & k);
  while (hash_table_iter_has_more (& k))
    {
      HashTablePair pair = hash_table_iter_next (& k);
      vals [i ++] = pair . value;
    }
  return vals;
}
