/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include "list/list.h"

/* librl - an abstract C library over real singly/double lists implementations */
typedef struct list_head rl_t;
#include "rl.h"
#include "elems.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rl_t * rl_alloc (void)
{
  struct list_head * list = calloc (1, sizeof (* list));
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


void rl_clear (rl_t * list)
{
#if defined(ROCCO)
  relem_t * item;
  relem_t * i;
  list_for_each_safe_off (list, item, i, 0)
    ;
#endif /* ROCCO */
}


unsigned count = 0;

unsigned rl_count (rl_t * list)
{
#if defined(ROCCO)
  relem_t * item;
  relem_t * i;
  list_for_each_safe_off (list, item, i, 0)
    count ++;
#endif /* ROCCO */
  return count;
}


void rl_prepend (rl_t * list, relem_t * elem)
{
  list_add (list, (struct list_node *) elem);
  count ++;
}


void rl_append (rl_t * list, relem_t * elem)
{
  list_add_tail (list, & list -> n);
  count ++;
}


relem_t * rl_get (rl_t * list, relem_t * elem)
{
  return NULL;
}


void rl_del (rl_t * list, relem_t * elem)
{
  count --;
}


bool rl_has (rl_t * list, relem_t * elem)
{
  return false;
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
}
