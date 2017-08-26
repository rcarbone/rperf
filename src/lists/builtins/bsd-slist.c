/* System headers */
#include <stdlib.h>


/* This implementation uses <sys/queue.h> macros from 4.4BSD */
#include <sys/queue.h>


/* Project headers */

/* librl - an abstract C library over real list implementations */
struct relem;
typedef SLIST_HEAD (, relem) rl_t;
#include "rl.h"

#include "elems.h"
#include "safe.h"


static relem_t * rl_last (rl_t * list)
{
  relem_t * elem = SLIST_FIRST (list);
  relem_t * last = NULL;
  while (elem)
    {
      last = elem;
      elem = SLIST_NEXT (elem, shead);
    }
  return last;
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rl_t * rl_alloc (void)
{
  rl_t * list = calloc (1, sizeof (* list));
  SLIST_INIT (list);

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
  SLIST_FOREACH (elem, list, shead)
    ;
#endif /* notused */
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  relem_t * elem;
  for (elem = SLIST_FIRST (list); elem; elem = SLIST_NEXT (elem, shead))
    count ++;
  return count;
}


void rl_prepend (rl_t * list, void * elem)
{
  SLIST_INSERT_HEAD (list, (relem_t *) elem, shead);
}


void rl_append (rl_t * list, void * elem)
{
  if (SLIST_EMPTY (list))
    SLIST_INSERT_HEAD (list, (relem_t *) elem, shead);
  else 
    SLIST_INSERT_AFTER (rl_last (list), (relem_t *) elem, shead);
}


void * rl_get (rl_t * list, void * arg)
{
  relem_t * elem;
  for (elem = SLIST_FIRST (list); elem; elem = SLIST_NEXT (elem, shead))
    if (elem == arg)
      return elem;
  return NULL;
}


void rl_del (rl_t * list, void * arg)
{
  relem_t * elem;
  for (elem = SLIST_FIRST (list); elem; elem = SLIST_NEXT (elem, shead))
    if (elem == arg)
      SLIST_REMOVE (list, elem, relem, shead);
}


bool rl_has (rl_t * list, void * elem)
{
  return rl_get (list, elem);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * elem;
  SLIST_FOREACH (elem, list, shead)
    fn (data);
}
