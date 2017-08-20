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

#if defined(ROCCO)
/* Allocate and populate a list inserting argc elements at the list head */
static rl_t * grow_head (unsigned argc, relem_t * argv [])
{
  rl_t * list = rl_alloc ();
  unsigned i;
  for (i = 0; i < argc; i ++)
    rl_prepend (list, argv [i]);
  return list;
}


/* Allocate and populate a list inserting argc elements at the list tail */
static rl_t * grow_tail (unsigned argc, relem_t * argv [])
{
  rl_t * list = rl_alloc ();
  unsigned i;
  for (i = 0; i < argc; i ++)
    rl_prepend (list, argv [i]);
  return list;
}
#endif /* ROCCO */


/* Allocate and populate a list inserting argc elements at the list head */
rtime_t rlsuite_grow_head (unsigned argc, relem_t * argv [])
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
rtime_t rlsuite_grow_tail (unsigned argc, relem_t * argv [])
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
