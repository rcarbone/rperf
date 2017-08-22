/* System headers */
#include <stdlib.h>


/* The implementation */
#include <sys/queue.h>


/* Project headers */
#include "elems.h"
#include "safe.h"


/* librl - an abstract C library over real list implementations */
typedef TAILQ_HEAD (, relem) rl_t;
#include "rl.h"


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


void rl_clear (rl_t * list)
{
  relem_t * elem;
  TAILQ_FOREACH (elem, list, tailq)
    ;
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  relem_t * elem;
  for (elem = TAILQ_FIRST (list); elem; elem = TAILQ_NEXT (elem, tailq))
    count ++;
  return count;
}


void rl_prepend (rl_t * list, relem_t * elem)
{
  TAILQ_INSERT_HEAD (list, elem, tailq);
}


void rl_append (rl_t * list, relem_t * elem)
{
  if (TAILQ_EMPTY (list))
    TAILQ_INSERT_HEAD (list, elem, tailq);
  else 
    TAILQ_INSERT_TAIL (list, elem, tailq);
}


relem_t * rl_get (rl_t * list, relem_t * arg)
{
  relem_t * elem;
  for (elem = TAILQ_FIRST (list); elem; elem = TAILQ_NEXT (elem, tailq))
    if (elem == arg)
      return elem;
  return NULL;
}


void rl_del (rl_t * list, relem_t * arg)
{
  relem_t * elem;
  for (elem = TAILQ_FIRST (list); elem; elem = TAILQ_NEXT (elem, tailq))
    if (elem == arg)
      TAILQ_REMOVE (list, elem, tailq);
}


bool rl_has (rl_t * list, relem_t * elem)
{
  return rl_get (list, elem);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * elem;
  TAILQ_FOREACH (elem, list, tailq)
    fn (data);
}
