/* System headers */
#include <stdlib.h>


/* The implementation */
#include <sys/queue.h>


/* Project headers */
#include "elems.h"
#include "safe.h"


/* librl - an abstract C library over real list implementations */
typedef STAILQ_HEAD (, relem) rl_t;
#include "rl.h"


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


void rl_clear (rl_t * list)
{
  relem_t * elem;
  STAILQ_FOREACH (elem, list, stailq)
    ;
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  relem_t * elem;
  for (elem = STAILQ_FIRST (list); elem; elem = STAILQ_NEXT (elem, stailq))
    count ++;
  return count;
}


void rl_prepend (rl_t * list, relem_t * elem)
{
  STAILQ_INSERT_HEAD (list, elem, stailq);
}


void rl_append (rl_t * list, relem_t * elem)
{
  if (STAILQ_EMPTY (list))
    STAILQ_INSERT_HEAD (list, elem, stailq);
  else 
    STAILQ_INSERT_TAIL (list, elem, stailq);
}


relem_t * rl_get (rl_t * list, relem_t * arg)
{
  relem_t * elem;
  for (elem = STAILQ_FIRST (list); elem; elem = STAILQ_NEXT (elem, stailq))
    if (elem == arg)
      return elem;
  return NULL;
}


void rl_del (rl_t * list, relem_t * arg)
{
  relem_t * elem;
  for (elem = STAILQ_FIRST (list); elem; elem = STAILQ_NEXT (elem, stailq))
    if (elem == arg)
      STAILQ_REMOVE (list, elem, relem, stailq);
}


bool rl_has (rl_t * list, relem_t * elem)
{
  return rl_get (list, elem);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * elem;
  STAILQ_FOREACH (elem, list, stailq)
    fn (data);
}
