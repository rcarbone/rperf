/* System headers */
#include <stdio.h>


/* The implementation */
#include <sys/queue.h>


/* Project headers */

/* librl - an abstract C library over real list implementations */
struct relem;
typedef STAILQ_HEAD (xxx, relem) rl_t;
#include "rl-api.h"

#include "elems.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static relem_t * rl_last (rl_t * list)
{
  relem_t * elem = STAILQ_FIRST (list);
  relem_t * last = NULL;
  while (elem)
    {
      last = elem;
      elem = STAILQ_NEXT (elem, stailq);
    }
  return last;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rl_t * rl_alloc (void)
{
  rl_t * list = calloc (1, sizeof (* list));
  STAILQ_INIT (list);
  return list;
}


void rl_free (rl_t * list)
{
  safefree (list);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * elem;
  STAILQ_FOREACH (elem, list, stailq)
    if (fn)
      fn (data);
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  relem_t * elem;
  for (elem = STAILQ_FIRST (list); elem; elem = STAILQ_NEXT (elem, stailq))
    count ++;
  return count;
}


void rl_prepend (rl_t * list, void * elem)
{
  STAILQ_INSERT_HEAD (list, (relem_t *) elem, stailq);
}


void rl_append (rl_t * list, void * elem)
{
  if (STAILQ_EMPTY (list))
    STAILQ_INSERT_HEAD (list, (relem_t *) elem, stailq);
  else 
    STAILQ_INSERT_TAIL (list, (relem_t *) elem, stailq);
}


void * rl_head (rl_t * list)
{
  return STAILQ_FIRST (list);
}


void * rl_tail (rl_t * list)
{
  return rl_last (list);
}


void * rl_get (rl_t * list, void * elem)
{
  relem_t * item;
  for (item = STAILQ_FIRST (list); item; item = STAILQ_NEXT (item, stailq))
    if (item == elem)
      return item;
  return NULL;
}


void rl_del (rl_t * list, void * elem)
{
  relem_t * item = rl_get (list, elem);
  if (item)
    STAILQ_REMOVE (list, item, relem, stailq);
}
