/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include "glib.h"

/* librhash - an abstract C library over real hash tables */
typedef GHashTable rhash_t;
#include "rhash.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

typedef struct
{
  void (* fn) (void *);
  void * data;

} func_t;


/* Callback to iterate over the hash table to search for a key */
static void myforeach (gpointer key, gpointer value, gpointer arg)
{
  func_t * func = arg;

  if (func)
    func -> fn (func -> data);
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rhash_t * rhash_alloc (unsigned size)
{
  return g_hash_table_new (g_str_hash, g_str_equal);
}


void rhash_free (rhash_t * ht)
{
  g_hash_table_destroy (ht);
}


void rhash_clear (rhash_t * ht)
{
  g_hash_table_remove_all (ht);
}


unsigned rhash_count (rhash_t * ht)
{
  return g_hash_table_size (ht);
}


void rhash_set (rhash_t * ht, char * key, void * val)
{
  g_hash_table_insert (ht, (gpointer) key, (gpointer) val);
}


void * rhash_get (rhash_t * ht, char * key)
{
  return g_hash_table_lookup (ht, key);
}


void rhash_del (rhash_t * ht, char * key)
{
  g_hash_table_remove (ht, key);
}


bool rhash_has (rhash_t * ht, char * key)
{
  return g_hash_table_contains (ht, key);
}


void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data)
{
  func_t fun = { .fn = fn, .data = data };
  g_hash_table_foreach (ht, myforeach, & fun);
}


char ** rhash_keys (rhash_t * ht)
{
  char ** keys = calloc (rhash_count (ht) + 1, sizeof (char *));
  GList * g_keys = g_hash_table_get_keys (ht);
  unsigned i = 0;
  GList * l;
  for (l = g_list_first (g_keys); l; l = g_list_next (l))
    keys [i ++] = l -> data;
  g_list_free (g_keys);
  return keys;
}


void ** rhash_vals (rhash_t * ht)
{
  void ** vals = calloc (rhash_count (ht) + 1, sizeof (char *));
  GList * g_vals = g_hash_table_get_values (ht);
  unsigned i = 0;
  GList * l;
  for (l = g_list_first (g_vals); l; l = g_list_next (l))
    vals [i ++] = l -> data;
  g_list_free (g_vals);
  return vals;
}
