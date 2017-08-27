/* Project headers */
#define NEED_RL_TYPEDEF
#include "rl.h"
#include "rltest.h"
#include "rlsuite.h"
#include "vargv.h"
#include "elems.h"
#include "rwall.h"
#include "support.h"


/* === Implementation of the built-in Test Suite === */


/* Callback to iterate over the list elements */
static void addone_cb (void * x)
{
  (* (unsigned *) x) ++;
}


/* Allocate and populate a list inserting argc elements at the list head */
static rl_t * grow_head (unsigned argc, void * argv [])
{
  rl_t * list = rl_alloc ();
  unsigned i;
  for (i = 0; i < argc; i ++)
    rl_prepend (list, argv [i]);
  return list;
}


#if defined(ROCCO)
/* Allocate and populate a list inserting argc elements at the list tail */
static rl_t * grow_tail (unsigned argc, void * argv [])
{
  rl_t * list = rl_alloc ();
  unsigned i;
  for (i = 0; i < argc; i ++)
    rl_prepend (list, argv [i]);
  return list;
}
#endif /* ROCCO */


/* Allocate and populate a list inserting argc elements at the list head */
rtime_t rlsuite_grow_head (unsigned argc, void * argv [])
{
  rl_t * list = rl_alloc ();
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rl_prepend (list, argv [i]);
  t2 = nswall ();
  i = rl_count (list);
  rl_free (list);
  return i == argc ? t2 - t1 : 0;
}


/* Allocate and populate a list inserting argc elements at the list tail */
rtime_t rlsuite_grow_tail (unsigned argc, void * argv [])
{
  rl_t * list = rl_alloc ();
  rtime_t t1;
  rtime_t t2;
  unsigned i;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rl_append (list, argv [i]);
  t2 = nswall ();
  i = rl_count (list);
  rl_free (list);
  return i == argc ? t2 - t1 : 0;
}


/* Iterate over all the list elements (forward) */
rtime_t rlsuite_iterate (unsigned argc, void * argv [])
{
  rl_t * list = grow_head (argc, argv);
  unsigned n = 0;
  rtime_t t1;
  rtime_t t2;
  t1 = nswall ();
  rl_foreach (list, addone_cb, & n);
  t2 = nswall ();
  rl_free (list);
  return n == argc ? t2 - t1 : 0;
}


/* Delete from the head of the list all the elements one-by-one */
rtime_t rlsuite_pop_head (unsigned argc, void * argv [])
{
  rl_t * list = grow_head (argc, argv);
  unsigned n = 0;
  unsigned i;
  rtime_t t1;
  rtime_t t2;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rl_del (list, rl_head (list));
  t2 = nswall ();
  n = rl_count (list);
  rl_free (list);
  return n == 0 ? t2 - t1 : 0;
}


rtime_t rlsuite_pop_tail (unsigned argc, void * argv [])
{
  rl_t * list = grow_head (argc, argv);
  unsigned n = 0;
  unsigned i;
  rtime_t t1;
  rtime_t t2;
  t1 = nswall ();
  for (i = 0; i < argc; i ++)
    rl_del (list, rl_tail (list));
  t2 = nswall ();
  n = rl_count (list);
  rl_free (list);
  return n == 0 ? t2 - t1 : 0;
}
