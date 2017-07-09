/*
 * All the Test Suite are here grouped together in a static table.
 *
 * Each implementation of a single Test Suite operates with 'n' different unique keys.
 */


/* Project headers */
typedef struct rht rht_t;
#include "rht.h"

#include "varrays.h"
#include "args.h"
#include "rwall.h"
#include "rctype.h"
#include "support.h"
#include "datasets.h"
#include "rtest.h"


/* The local identifiers for the Test Suite to run (indexed by keys) */
typedef enum
{
  RSUITE_GROW    = 0x01,  /* Populate an empty container                          */
  RSUITE_HIT     = 0x02,  /* Populate and find all existing objects one-by-one    */
  RSUITE_MISS    = 0x03,  /* Populate and search for non existing objects         */
  RSUITE_DELETE  = 0x04,  /* Populate and delete all existing objects one-by-one  */
  RSUITE_REPLACE = 0x05,  /* Populate and replace all existing objects one-by-one */
  RSUITE_KBENCH  = 0x06,  /* Delete if found, add otherwise objects one-by-one    */

} rsuite_id_t;


/* Forward */
rsuite_f rsuite_grow;
rsuite_f rsuite_hit;
rsuite_f rsuite_miss;
rsuite_f rsuite_delete;
rsuite_f rsuite_replace;
rsuite_f rsuite_kbench;


/* All the Test Suite in an array */
static rtest_t rsuite_builtins [] =
{
  { RSUITE_GROW,    "grow",    "Add objects to an empty container", NULL, rsuite_grow    },
  { RSUITE_HIT,     "hit",     "Find existing objects",             NULL, rsuite_hit     },
  { RSUITE_MISS,    "miss",    "Search non existing objects",       NULL, rsuite_miss    },
  { RSUITE_DELETE,  "delete",  "Delete existing objects",           NULL, rsuite_delete  },
  { RSUITE_REPLACE, "replace", "Replace existing objects",          NULL, rsuite_replace },
  { RSUITE_KBENCH,  "kbench",  "Delete if found, add otherwise",    NULL, rsuite_kbench  },
};
#define RSUITE_NO (sizeof (rsuite_builtins) / sizeof (* rsuite_builtins))


/* === Implementation of Test Suite === */

/* Allocate and populate a hash table with n objects */
static rht_t * populate (unsigned argc, robj_t * argv [])
{
  rht_t * ht = rht_alloc (argc);
  unsigned i;
  for (i = 0; i < argc; i ++)
    rht_set (ht, argv [i] -> skey, & argv [i] -> ukey);
  return ht;
}


/* Allocate and populate all the objects starting with an empty a hash table */
rtime_t rsuite_grow (unsigned argc, robj_t * argv [])
{
  rht_t * ht = rht_alloc (argc);
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rht_set (ht, argv [i] -> skey, & argv [i] -> ukey);
  t2 = nswall ();
  i = rht_count (ht);
  rht_free (ht);
  return i == argc ? t2 - t1 : 0;
}


/* Find and dereference with success all the objects */
rtime_t rsuite_hit (unsigned argc, robj_t * argv [])
{
  rht_t * ht = populate (argc, argv);
  unsigned hit = 0;
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  void * found;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    {
      found = rht_get (ht, argv [i] -> skey);
      if (found && found == & argv [i] -> ukey)
	hit ++;
    }
  t2 = nswall ();
  rht_free (ht);
  return hit == argc ? t2 - t1 : 0;
}


/* Find with failure all the objects */
rtime_t rsuite_miss (unsigned argc, robj_t * argv [])
{
  rht_t * ht = populate (argc, argv);
  unsigned missed = 0;
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    if (! rht_get (ht, argv [i] -> smiss))
      missed ++;
  t2 = nswall ();
  rht_free (ht);
  return missed == argc ? t2 - t1 : 0;
}


/* Remove and dereference all the objects */
rtime_t rsuite_delete (unsigned argc, robj_t * argv [])
{
  rht_t * ht = populate (argc, argv);
  unsigned deleted;
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rht_del (ht, argv [i] -> skey);
  deleted = argc - rht_count (ht);
  t2 = nswall ();
  rht_free (ht);
  return deleted == argc ? t2 - t1 : 0;
}


/* Find, delete and reinsert with a different key all the objects */
rtime_t rsuite_replace (unsigned argc, robj_t * argv [])
{
  rht_t * ht = populate (argc, argv);
  unsigned replaced;
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  void * found;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    {
      found = rht_get (ht, argv [i] -> skey);
      if (found && found == & argv [i] -> ukey)
	{
	  rht_del (ht, argv [i] -> skey);
	  found = rht_get (ht, argv [i] -> smiss);
	  if (! found)
	    rht_set (ht, argv [i] -> smiss, & argv [i] -> ukey);
	}
    }
  t2 = nswall ();
  replaced = rht_count (ht);
  rht_free (ht);
  return replaced == argc ? t2 - t1 : 0;
}


/* Add if not found, delete otherwise */
rtime_t rsuite_kbench (unsigned argc, robj_t * argv [])
{
  rht_t * ht = populate (argc, argv);
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  void * found;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    {
      found = rht_get (ht, argv [i] -> skey);
      if (found && found == & argv [i] -> ukey)
	rht_del (ht, argv [i] -> skey);
      else
	rht_set (ht, argv [i] -> smiss, & argv [i] -> ukey);
    }
  t2 = nswall ();
  rht_free (ht);
  return t2 - t1;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Rendering functions */

/* Max name length */
static unsigned rsuite_maxn (rtest_t * argv [])
{
  unsigned n = 0;
  while (argv && * argv)
    {
      n = RMAX (n, strlen ((* argv) -> name));
      argv ++;
    }
  return n;
}


static void rsuite_run_one (rtest_t * rtest, unsigned argc, robj_t * argv [], unsigned n, unsigned seq, unsigned maxn)
{
  rtime_t spent;

  print_dots (rtest -> name, "Running", n, seq, maxn);

  spent = rtest -> suite (argc, argv);
  if (spent)
    printf ("Ok - %s\n", ns2a (spent));
  else
    printf ("No\n");
}


/* Return the # of Test Suite */
unsigned rsuite_no (void)
{
  return RSUITE_NO;
}


/* Return the handle of the first Test Suite */
rtest_t * rsuite_first (void)
{
  return & rsuite_builtins [0];
}


/* Return the handle of the last Test Suite */
rtest_t * rsuite_last (void)
{
  return & rsuite_builtins [RSUITE_NO - 1];
}


/* Return the handle of the next Test Suite */
rtest_t * rsuite_next (void)
{
  static unsigned i = 0;
  return & rsuite_builtins [i ++ % RSUITE_NO];
}


/* Return the handle of the next Test Suite randomly generated */
rtest_t * rsuite_next_rnd (void)
{
  return & rsuite_builtins [rrand (RSUITE_NO)];
}


/* Looukup for an Test Suite by index */
rtest_t * rsuite_find_at (unsigned i)
{
  return i < RSUITE_NO ? & rsuite_builtins [i] : NULL;
}


/* Looukup for an Test Suite by its unique id */
rtest_t * rsuite_find_by_id (unsigned id)
{
  unsigned i;
  for (i = 0; i < RSUITE_NO; i ++)
    if (rsuite_builtins [i] . id == id)
      return & rsuite_builtins [i];
  return NULL;
}


/* Looukup for an Test Suite by its unique name */
rtest_t * rsuite_find_by_name (char * name)
{
  unsigned i;
  for (i = 0; name && i < RSUITE_NO; i ++)
    if (! strcmp (rsuite_builtins [i] . name, name))
      return & rsuite_builtins [i];
  return NULL;
}


/* Return all the Test Suite names in an array in the same order they were defined */
char ** rsuite_names (void)
{
  unsigned i;
  char ** all = NULL;
  for (i = 0; i < RSUITE_NO; i ++)
    all = argsmore (all, rsuite_builtins [i] . name);
  return all;
}


/* Return all the Test Suite in an array in the same order they were defined */
rtest_t ** rsuite_all (void)
{
  unsigned i;
  rtest_t ** all = NULL;
  for (i = 0; i < RSUITE_NO; i ++)
    all = arrmore (all, & rsuite_builtins [i], rtest_t);
  return all;
}


/* Return all the Test Suite in an array in the same order they were defined starting at given offset */
rtest_t ** rsuite_all_n (unsigned n)
{
  rtest_t ** all;
  unsigned i;

  n %= RSUITE_NO;

  for (i = 0; i < n; i ++)
    rsuite_next ();

  all = arrmore (NULL, rsuite_next (), rtest_t);
  while (arrlen (all) != rsuite_no ())
    all = arrmore (all, rsuite_next (), rtest_t);
  return all;
}


rtest_t ** rsuite_all_rnd (void)
{
  return (rtest_t **) varnd (rsuite_no (), (void **) rsuite_all ());
}


rtest_t * rsuite_valid (char * id)
{
  return isnumeric (id) && atoi (id) ? rsuite_find_at (atoi (id) - 1) : rsuite_find_by_name (id);
}


/* Run the Test Suite included in argv[] */
void rsuite_run (rtest_t * suite [], unsigned argc, robj_t * argv [])
{
  unsigned maxn = rsuite_maxn (suite);
  unsigned n    = digits (arrlen (suite));
  unsigned seq  = 0;

  while (suite && * suite)
    rsuite_run_one (* suite ++, argc, argv, n, ++ seq, maxn);
}
