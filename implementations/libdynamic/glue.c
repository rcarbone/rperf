/* System headers */
#include <stdio.h>


/* The implementation */
#include "map.c"


/* librht - an abstract C library over real hash tables */
typedef map rht_t;
#include "rht.h"


/* Items to put in the hash table */
typedef struct
{
  char * key;
  void * val;

} dynobj_t;


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


/* The empty object */
static dynobj_t empty = { .key = NULL, .val = NULL };


/*
 * The set callback is called with a pointer to a source element from where
 * the element data is read, and a destination element where the data is written
 */
static void set_cb (map * map, void * d, void * s)
{
  dynobj_t * dst = d;
  dynobj_t * src = s;
  * dst = * src;
}


/* The hash callback is called with a pointer to a map element and should return a hash value of the key of the element */
static size_t hash_cb (map * ht, void * obj)
{
  return rht_python_hash (((dynobj_t *) obj) -> key);
}


/*
 * The equal callback is called with a pointer to two elements, element1 and element2,
 * and should return 1 if the elements are equal.
 */
static int equal_cb (map * ht, void * o1, void * o2)
{
  dynobj_t * obj1 = o1;
  dynobj_t * obj2 = o2;

  return obj1 -> key == obj2 -> key || (obj1 -> key && obj2 -> key && ! strcmp (obj1 -> key, obj2 -> key));
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  map * ht = calloc (1, sizeof (* ht));
  map_construct (ht, sizeof (dynobj_t), & empty, set_cb);   /* use the empty object */
  return ht;
}


void rht_free (rht_t * ht)
{
  map_destruct (ht, NULL, NULL);
  free (ht);
}


void rht_clear (rht_t * ht)
{
  map_clear (ht, equal_cb, set_cb, NULL);
}


unsigned rht_count (rht_t * ht)
{
  return map_size (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  dynobj_t obj [] = { { .key = key, .val = val } };
  map_insert (ht, obj, hash_cb, equal_cb, set_cb, NULL);
}


void * rht_get (rht_t * ht, char * key)
{
  dynobj_t obj [] = { { .key = key } };
  return ((dynobj_t *) map_at (ht, obj, hash_cb, equal_cb)) -> val;
}


void rht_del (rht_t * ht, char * key)
{
  dynobj_t obj [] = { { .key = key } };
  map_erase (ht, obj, hash_cb, equal_cb, set_cb, NULL);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  unsigned i = 0;
  for (i = 0; i < ht -> elements_capacity; i ++)
    if (! equal_cb (ht, map_element (ht, i), ht -> element_empty))
      fn (data);
}

char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  for (i = 0; i < ht -> elements_capacity; i ++)
    {
      dynobj_t * obj = map_element (ht, i);
      if (! equal_cb (ht, obj, ht -> element_empty))
	keys [i ++] = obj -> key;
    }
  printf ("ROCCO: n = %u - k = %u\n", rht_count (ht), valen (keys));
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  for (i = 0; i < ht -> elements_capacity; i ++)
    {
      dynobj_t * obj = map_element (ht, i);
      if (obj && ! equal_cb (ht, obj, ht -> element_empty))
	vals [i ++] = obj -> val;
    }
  return vals;
}
