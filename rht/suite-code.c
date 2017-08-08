/* Project headers */
typedef struct rht rht_t;
#include "rht.h"
#include "vargs.h"
#include "rwall.h"
#include "datasets.h"


/* === Implementation of the built-in Test Suite === */


/* Allocate and populate a hash table with n unique items */
static rht_t * populate (unsigned argc, robj_t * argv [])
{
  rht_t * ht = rht_alloc (argc);
  unsigned i;
  for (i = 0; i < argc; i ++)
    rht_set (ht, argv [i] -> skey, argv [i] -> pval);
  return ht;
}


/* Allocate and populate all the items starting with an empty a hash table */
rtime_t rsuite_grow (unsigned argc, robj_t * argv [])
{
  rht_t * ht = rht_alloc (argc);
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rht_set (ht, argv [i] -> skey, argv [i] -> pval);
  t2 = nswall ();
  i = rht_count (ht);
  rht_free (ht);
  return i == argc ? t2 - t1 : 0;
}


/* Find and dereference with success all the items */
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
      if (found && found == argv [i] -> pval)
	hit ++;
    }
  t2 = nswall ();
  rht_free (ht);
  return hit == argc ? t2 - t1 : 0;
}


/* Find with failure all the items */
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


/* Remove and dereference all the items */
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


/* Find, delete and reinsert with a different key all the items */
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
      if (found && found == argv [i] -> pval)
	{
	  rht_del (ht, argv [i] -> skey);
	  found = rht_get (ht, argv [i] -> smiss);
	  if (! found)
	    rht_set (ht, argv [i] -> smiss, argv [i] -> pval);
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
  rht_t * ht = populate (0, NULL);
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  void * found;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    {
      found = rht_get (ht, argv [i] -> skey);
      if (found && found == argv [i] -> pval)
	rht_del (ht, argv [i] -> skey);
      else
	rht_set (ht, argv [i] -> smiss, argv [i] -> pval);
    }
  t2 = nswall ();
  rht_free (ht);
  return t2 - t1;
}
