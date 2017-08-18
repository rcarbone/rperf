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
/* Allocate and populate a list with n unique elements */
static rl_t * populate (unsigned argc, relem_t * argv [])
{
  rl_t * list = rl_alloc ();
  unsigned i;
  for (i = 0; i < argc; i ++)
    rl_prepend (list, argv [i]);
  return list;
}
#endif /* ROCCO */


/* Allocate and populate all the elements starting with an empty list */
rtime_t rlsuite_grow (unsigned argc, relem_t * argv [])
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
