/*
 * The Suite for Tries implementations is here.
 *
 * All the functions are grouped together in a static table.
 */


/* Project headers */
#define NEED_RTRIE_TYPEDEF
#include "rtrie-api.h"
#include "rsuite.h"
#include "sargv.h"
#include "rwall.h"
#include "rctype.h"
#include "support.h"


/* The Suite in a static table */
static rsuite_t builtins [] =
{
  { "grow_seq", "Populate an empty container [unique sequential keys]", rsuite_grow_seq },
  { "grow_rnd", "Populate an empty container [unique random keys]",     rsuite_grow_rnd },
  { "hit_seq",  "Search one-by-one all existing [sequential keys]",     rsuite_hit_seq  },
  { "hit_rnd",  "Search one-by-one all existing [random keys]",         rsuite_hit_rnd  },

  { "pop",      "Remove all elements one-by-one", rsuite_pop     },

  { "iterate",  "Iterate over all trie elements", rsuite_iterate },
};
#define RSUITENO (sizeof (builtins) / sizeof (* builtins))


/* === Implementation of the builtin Suite === */


/* Callback to iterate over the trie elements */
static void addone_cb (void * x)
{
  (* (unsigned *) x) ++;
}


/* Allocate and populate a trie inserting argc elements */
static rtrie_t * populate (unsigned argc, void * argv [])
{
  rtrie_t * trie = rtrie_alloc ();
  unsigned i;
  for (i = 0; i < argc; i ++)
    rtrie_add (trie, argv [i]);
  return trie;
}


/* Allocate and populate a trie inserting argc elements in sequential order */
rtime_t rsuite_grow_seq (unsigned argc, void * argv [])
{
  rtrie_t * trie = rtrie_alloc ();
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rtrie_add (trie, argv [i]);
  t2 = nswall ();
  i = rtrie_count (trie);
  rtrie_free (trie);
  return i == argc ? t2 - t1 : 0;
}


/* Allocate and populate a trie inserting argc elements in random order */
rtime_t rsuite_grow_rnd (unsigned argc, void * argv [])
{
  rtrie_t * trie = rtrie_alloc ();
  unsigned * order = rndorder (argc);
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rtrie_add (trie, argv [order [i]]);
  t2 = nswall ();
  free (order);
  i = rtrie_count (trie);
  rtrie_free (trie);
  return i == argc ? t2 - t1 : 0;
}


/* Find and dereference with success all the items in sequential order */
rtime_t rsuite_hit_seq (unsigned argc, void * argv [])
{
  rtrie_t * trie = populate (argc, argv);
  unsigned hit = 0;
  unsigned i;
  rtime_t t1;
  rtime_t t2;
  void * found;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    {
      found = rtrie_get (trie, argv [i]);
      if (found /* && found == ((rnode_t *) argv [i]) -> foo */)           /* dereference */
	hit ++;
    }
  t2 = nswall ();
  rtrie_free (trie);
  return hit == argc ? t2 - t1 : 0;
}


/* Find and dereference with success all the items in random order */
rtime_t rsuite_hit_rnd (unsigned argc, void * argv [])
{
  rtrie_t * trie = populate (argc, argv);
  unsigned * order = rndorder (argc);
  unsigned hit = 0;
  unsigned i;
  rtime_t t1;
  rtime_t t2;
  void * found;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    {
      found = rtrie_get (trie, argv [order [i]]);
      if (found /* && found == ((rnode_t *) argv [order [i]]) -> foo */)           /* dereference */
	hit ++;
    }
  t2 = nswall ();
  free (order);
  rtrie_free (trie);
  return hit == argc ? t2 - t1 : 0;
}


/* Delete from the head of the trie all the elements one-by-one */
rtime_t rsuite_pop (unsigned argc, void * argv [])
{
  rtrie_t * trie = populate (argc, argv);
  unsigned n = 0;
  unsigned i;
  rtime_t t1;
  rtime_t t2;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rtrie_del (trie, argv [i]);
  t2 = nswall ();
  n = rtrie_count (trie);
  rtrie_free (trie);
  return n == 0 ? t2 - t1 : 0;
}


/* Iterate over all the trie elements */
rtime_t rsuite_iterate (unsigned argc, void * argv [])
{
  rtrie_t * trie = populate (argc, argv);
  unsigned n = 0;
  rtime_t t1;
  rtime_t t2;
  t1 = nswall ();
  rtrie_foreach (trie, addone_cb, & n);
  t2 = nswall ();
  rtrie_free (trie);
  return n == argc ? t2 - t1 : 0;
}


/* Return the # of items */
unsigned rtrie_suite_argc (void)
{
  return RSUITENO;
}


/* Return the table of the Suite */
rsuite_t * rtrie_suite_argv (void)
{
  return builtins;
}
