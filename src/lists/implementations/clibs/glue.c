/* System headers */
#include <stdlib.h>


/* The implementation */
#include "list.c"
#include "list_node.c"
#include "list_iterator.c"


/* Project headers */

/* librl - an abstract C library over real singly/double lists implementations */
typedef tj_list_t rl_t;
#include "rl-api.h"

#include "elems.h"


rl_t * rl_alloc (void)
{
  return list_new ();
}


void rl_free (rl_t * list)
{
  safefree (list);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  list_iterator_t * it = list_iterator_new (list, TJ_LIST_HEAD);
  while (list_iterator_next (it))
    if (fn)
      fn (data);
  list_iterator_destroy (it);
}


unsigned rl_count (rl_t * list)
{
  return list -> len;
}


void rl_prepend (rl_t * list, void * arg)
{
  relem_t * elem = arg;
  elem -> clibs . val = arg;
  list_lpush (list, & elem -> clibs);
}


void rl_append (rl_t * list, void * arg)
{
  relem_t * elem = arg;
  elem -> clibs . val = arg;
  list_rpush (list, & elem -> clibs);
}


void * rl_head (rl_t * list)
{
  return list_at (list, 0);
}


void * rl_tail (rl_t * list)
{
  return list_at (list, rl_count (list) - 1);
}


void * rl_get (rl_t * list, void * elem)
{
  return list_find (list, elem);
}


void rl_del (rl_t * list, void * arg)
{
  if (rl_get (list, arg))
    {
      relem_t * elem = arg;
      elem -> clibs . val = arg;
      list_remove (list, & elem -> clibs);
    }
}
