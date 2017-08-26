/* System headers */
#include <stdlib.h>


/* The implementation */
#include <sys/queue.h>


/* Project headers */

/* librl - an abstract C library over real list implementations */
struct relem;
typedef TAILQ_HEAD (, relem) rl_t;
#include "rl.h"

#include "elems.h"
#include "safe.h"


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
#if defined(notused)
  relem_t * elem;
  TAILQ_FOREACH (elem, list, tailq)
    ;
#endif /* notused */
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


void * rl_get (rl_t * list, void * arg)
{
  relem_t * elem;
  for (elem = TAILQ_FIRST (list); elem; elem = TAILQ_NEXT (elem, tailq))
    if (elem == arg)
      return elem;
  return NULL;
}


void rl_del (rl_t * list, void * arg)
{
  relem_t * elem;
  for (elem = TAILQ_FIRST (list); elem; elem = TAILQ_NEXT (elem, tailq))
    if (elem == arg)
      TAILQ_REMOVE (list, elem, tailq);
}


bool rl_has (rl_t * list, void * elem)
{
  return rl_get (list, elem);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * elem;
  TAILQ_FOREACH (elem, list, tailq)
    fn (data);
}
