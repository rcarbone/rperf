/* System headers */
#include <stdlib.h>
#include <sys/queue.h>


/* Project headers */
#include "elems.h"
#include "safe.h"


/* librl - an abstract C library over real list implementations */
typedef SLIST_HEAD (, relem) rl_t;
#include "rl.h"


static relem_t * rl_last (rl_t * list)
{
  relem_t * elem = SLIST_FIRST (list);
  relem_t * last = NULL;
  while (elem)
    {
      last = elem;
      elem = SLIST_NEXT (elem, head);
    }
  return last;
}


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
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  relem_t * elem;
  for (elem = SLIST_FIRST (list); elem; elem = SLIST_NEXT (elem, head))
    count ++;
  return count;
}


void rl_prepend (rl_t * list, relem_t * elem)
{
  SLIST_INSERT_HEAD (list, elem, head);
}


void rl_append (rl_t * list, relem_t * elem)
{
  if (SLIST_EMPTY (list))
    SLIST_INSERT_HEAD (list, elem, head);
  else 
    SLIST_INSERT_AFTER (rl_last (list), elem, head);
}


relem_t * rl_get (rl_t * list, relem_t * arg)
{
  relem_t * elem;
  for (elem = SLIST_FIRST (list); elem; elem = SLIST_NEXT (elem, head))
    if (elem == arg)
      return elem;
  return NULL;
}


void rl_del (rl_t * list, relem_t * arg)
{
  relem_t * elem;
  for (elem = SLIST_FIRST (list); elem; elem = SLIST_NEXT (elem, head))
    if (elem == arg)
      SLIST_REMOVE (list, arg, relem, head);
}


bool rl_has (rl_t * list, relem_t * elem)
{
  return rl_get (list, elem);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * elem;
  SLIST_FOREACH (elem, list, head)
    fn (data);
}
