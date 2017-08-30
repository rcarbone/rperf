/* System headers */
#include <assert.h>

/* Project headers */
#define NEED_RL_TYPEDEF
#include "rl-api.h"
#include "vargv.h"
#include "elems.h"


/* === Implementation of the built-in Unit Tests === */


/* Callback to iterate over the list elements */
static void addone_cb (void * x)
{
  (* (unsigned *) x) ++;
}


/* Allocate and populate a list inserting argc elements at the list head */
static rl_t * headpopulate (unsigned argc, relem_t * argv [])
{
  rl_t * list = rl_alloc ();
  unsigned i;
  assert (list);
  assert (rl_count (list) == 0);
  for (i = 0; i < argc; i ++)
    rl_prepend (list, argv [i]);
  assert (rl_count (list) == argc);
  return list;
}


/* Allocate and populate a list inserting argc elements at the list tail */
static rl_t * tailpopulate (unsigned argc, relem_t * argv [])
{
  rl_t * list = rl_alloc ();
  unsigned i;
  assert (list);
  assert (rl_count (list) == 0);
  for (i = 0; i < argc; i ++)
    rl_append (list, argv [i]);
  assert (rl_count (list) == argc);
  return list;
}


unsigned alloc_free (unsigned argc)
{
  rl_t * list = headpopulate (0, NULL);
  assert (list);
  assert (rl_count (list) == 0);
  rl_free (list);
  return argc;
}


unsigned alloc_append_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  rl_t * list = tailpopulate (argc, argv);
  rl_free (list);
  rmelems (argv);
  return argc;
}


unsigned alloc_prepend_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  rl_t * list = headpopulate (argc, argv);
  rl_free (list);
  rmelems (argv);
  return argc;
}


unsigned alloc_iterate_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  rl_t * list = headpopulate (argc, argv);
  unsigned count = 0;
  rl_foreach (list, addone_cb, & count);
  assert (rl_count (list) == count);
  rl_free (list);
  rmelems (argv);
  return argc;
}


unsigned alloc_count_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  rl_t * list = headpopulate (argc, argv);
  unsigned count = rl_count (list);
  rl_free (list);
  rmelems (argv);
  return count;
}


unsigned alloc_found_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  rl_t * list = headpopulate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    {
      relem_t * elem = rl_get (list, argv [i]);
      assert (elem);
    }
  rl_free (list);
  rmelems (argv);
  return argc;
}


unsigned alloc_miss_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  rl_t * list = headpopulate (argc, argv);
  unsigned i;
  relem_t miss;
  for (i = 0; i < argc; i ++)
    assert (! rl_get (list, & miss));
  rl_free (list);
  rmelems (argv);
  return argc;
}


unsigned alloc_delete_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  rl_t * list = headpopulate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    rl_del (list, argv [i]);
  assert (rl_count (list) == 0);
  rl_free (list);
  rmelems (argv);
  return argc;
}


unsigned alloc_missed_free (unsigned argc)
{
  relem_t ** argv = mkelems (argc);
  relem_t ** miss = mkelems (argc);
  rl_t * list = headpopulate (argc, argv);
  unsigned i;
  for (i = 0; i < argc; i ++)
    rl_del (list, miss [i]);
  assert (rl_count (list) == argc);
  rl_free (list);
  rmelems (miss);
  rmelems (argv);
  return argc;
}
