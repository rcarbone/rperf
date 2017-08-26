/* System headers */
#include <stdlib.h>


/* The implementation */
#include "list/list.h"
#undef LIST_HEAD


/* Project headers */

/* librl - an abstract C library over real singly/double lists implementations */
typedef struct list_head rl_t;
#include "rl.h"

#include "elems.h"


rl_t * rl_alloc (void)
{
  rl_t * list = calloc (1, sizeof (* list));
  list_head_init (list);
  return list;
}


void rl_free (rl_t * list)
{
  relem_t * elem;
  relem_t * it;
  list_for_each_safe_off (list, elem, it, 0)
    ;
  free (list);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * item;
  list_for_each (list, item, ccan)
    fn (data);
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  relem_t * item;
  list_for_each (list, item, ccan)
    count ++;
  return count;
}


void rl_prepend (rl_t * list, void * elem)
{
  list_add (list, & ((relem_t *) elem) -> ccan);
}


void rl_append (rl_t * list, void * elem)
{
  list_add_tail (list, & ((relem_t *) elem) -> ccan);
}


void * rl_get (rl_t * list, void * elem)
{
  relem_t * item;
  list_for_each (list, item, ccan)
    if (item == elem)
      return item;
  return NULL;
}


void rl_del (rl_t * list, void * elem)
{
  relem_t * item;
  list_for_each (list, item, ccan)
    if (item == elem)
      list_del (& item -> ccan);
}
