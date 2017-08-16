/* System headers */
#include <assert.h>

/* Project headers */
#define NEED_RL_TYPEDEF
#include "rl.h"
#include "vargv.h"
#include "elems.h"


/* === Implementation of the built-in Unit Tests === */


/* Allocate and populate a list with n elements */
static rl_t * populate (unsigned argc, relem_t * argv [])
{
  rl_t * ht = rl_alloc (argc);
  unsigned i;
  assert (ht);
  assert (rl_count (ht) == 0);
  for (i = 0; i < argc; i ++)
    {
      rl_prepend (ht, argv [i]);
      // assert (rl_count (ht) == i + 1);
    }
  assert (rl_count (ht) == argc);
  return ht;
}


unsigned alloc_free (unsigned argc)
{
  rl_t * ht = rl_alloc (argc);
  assert (ht);
  assert (rl_count (ht) == 0);
  rl_free (ht);
  return argc;
}


unsigned alloc_append_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  rl_t * ht = populate (argc, argv);
  rl_free (ht);
  rmelems (argv);
  return argc;
}


unsigned alloc_prepend_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  rl_t * ht = populate (argc, argv);
  rl_free (ht);
  rmelems (argv);
  return argc;
}
