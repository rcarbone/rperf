/* System headers */
#include <stdio.h>


/* The implementation */
#include <sys/queue.h>


/* Project headers */

/* librl - an abstract C library over real list implementations */
struct relem;
typedef LIST_HEAD (xxx, relem) rl_t;
#include "rl-api.h"

#include "elems.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


static relem_t * rl_last (rl_t * list)
{
  relem_t * elem = LIST_FIRST (list);
  relem_t * last = NULL;
  while (elem)
    {
      last = elem;
      elem = LIST_NEXT (elem, head);
    }
  return last;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rl_t * rl_alloc (void)
{
  rl_t * list = calloc (1, sizeof (* list));
  LIST_INIT (list);

  return list;
}


void rl_free (rl_t * list)
{
  safefree (list);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * elem;
  LIST_FOREACH (elem, list, head)
    if (fn)
      fn (data);
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  relem_t * elem;
  for (elem = LIST_FIRST (list); elem; elem = LIST_NEXT (elem, head))
    count ++;
  return count;
}


void rl_prepend (rl_t * list, void * arg)
{
  relem_t * elem = arg;
  LIST_INSERT_HEAD (list, elem, head);
}


void rl_append (rl_t * list, void * arg)
{
  relem_t * elem = arg;
  if (LIST_EMPTY (list))
    LIST_INSERT_HEAD (list, elem, head);
  else 
    LIST_INSERT_AFTER (rl_last (list), elem, head);
}


void * rl_head (rl_t * list)
{
  return LIST_FIRST (list);
}


void * rl_tail (rl_t * list)
{
  return rl_last (list);
}


void * rl_get (rl_t * list, void * arg)
{
  relem_t * elem;
  for (elem = LIST_FIRST (list); elem; elem = LIST_NEXT (elem, head))
    if (elem == arg)
      return elem;
  return NULL;
}


void rl_del (rl_t * list, void * arg)
{
  relem_t * elem;
  for (elem = LIST_FIRST (list); elem; elem = LIST_NEXT (elem, head))
    if (elem == arg)
      LIST_REMOVE (elem, head);
}
