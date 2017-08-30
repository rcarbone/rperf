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
  return NULL;
}


void * rl_get (rl_t * list, void * arg)
{
  relem_t * elem;
  for (elem = STAILQ_FIRST (list); elem; elem = STAILQ_NEXT (elem, stailq))
    if (elem == arg)
      return elem;
  return NULL;
}


void rl_del (rl_t * list, void * arg)
{
  relem_t * elem;
  for (elem = STAILQ_FIRST (list); elem; elem = STAILQ_NEXT (elem, stailq))
    if (elem == arg)
      STAILQ_REMOVE (list, elem, relem, stailq);
}
