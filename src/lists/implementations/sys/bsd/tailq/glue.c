/* System headers */
#include <stdio.h>


/* The implementation */
#include <sys/queue.h>


/* Project headers */

/* librl - an abstract C library over real list implementations */
struct relem;
typedef TAILQ_HEAD (xxx, relem) rl_t;
#include "rl-api.h"

#include "elems.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rl_t * rl_alloc (void)
{
  rl_t * list = calloc (1, sizeof (* list));
  TAILQ_INIT (list);
  return list;
}


void rl_free (rl_t * list)
{
  safefree (list);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * elem;
  TAILQ_FOREACH (elem, list, tailq)
    if (fn)
      fn (data);
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  relem_t * elem;
  for (elem = TAILQ_FIRST (list); elem; elem = TAILQ_NEXT (elem, tailq))
    count ++;
  return count;
}


void rl_prepend (rl_t * list, void * elem)
{
  TAILQ_INSERT_HEAD (list, (relem_t *) elem, tailq);
}


void rl_append (rl_t * list, void * elem)
{
  if (TAILQ_EMPTY (list))
    TAILQ_INSERT_HEAD (list, (relem_t *) elem, tailq);
  else 
    TAILQ_INSERT_TAIL (list, (relem_t *) elem, tailq);
}


void * rl_head (rl_t * list)
{
  return TAILQ_FIRST (list);
}


void * rl_tail (rl_t * list)
{
  return TAILQ_LAST (list, xxx);
}


void * rl_get (rl_t * list, void * elem)
{
  relem_t * item;
  for (item = TAILQ_FIRST (list); item; item = TAILQ_NEXT (item, tailq))
    if (item == elem)
      return item;
  return NULL;
}


void rl_del (rl_t * list, void * elem)
{
  relem_t * item = rl_get (list, elem);
  if (item)
    TAILQ_REMOVE (list, item, tailq);
}
