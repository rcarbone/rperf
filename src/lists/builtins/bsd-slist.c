/* System headers */
#include <stdlib.h>
#include <sys/queue.h>


/* Project headers */
#include "elems.h"
#include "safe.h"


/* librl - an abstract C library over real list implementations */
typedef SLIST_HEAD (, relem) rl_t;
#include "rl.h"


rl_t * rl_alloc (void)
{
  /* list declaration and initialization */
  SLIST_HEAD (, relem) * list = calloc (1, sizeof (* list));
  SLIST_INIT (list);

  return (rl_t *) list;
  // return list;
}


void rl_free (rl_t * list)
{
  safefree (list);
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  struct relem * elem;
  for (elem = SLIST_FIRST (list); elem; elem = SLIST_NEXT (elem, head))
    count ++;
  return count;
}


void rl_append (rl_t * list, relem_t * elem)
{
  //  struct relem * next = NULL;
  // if (SLIST_EMPTY (list))
    SLIST_INSERT_HEAD (list, elem, head);
    // else 
    // SLIST_INSERT_AFTER (next, elem, head);
}


void rl_prepend (rl_t * list, relem_t * elem)
{
  SLIST_INSERT_HEAD (list, elem, head);
}
