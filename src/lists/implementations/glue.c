/* System headers */
#include <stdlib.h>


/* The implementation */
#include "ring.h"


/* Project headers */

/* librl - an abstract C library over real singly/double lists implementations */
typedef xxx rl_t;
#include "rl-api.h"

#include "elems.h"


rl_t * rl_alloc (void)
{
  rl_t * list = calloc (1, sizeof (* list));
  xxx_list_init (list);
  return list;
}


void rl_free (rl_t * list)
{
  safefree (list);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  xxx_node * node = xxx_list_head (list);
  while (node)
    {
      if (fn)
	fn (data);
      node = node -> next;
    }
}


unsigned rl_count (rl_t * list)
{
  return xxx_list_count (list);
}


void rl_prepend (rl_t * list, void * elem)
{
  xxx_list_insert_head (list, & ((relem_t *) elem) -> tommy, elem);
}


void rl_append (rl_t * list, void * elem)
{
  xxx_list_insert_tail (list, & ((relem_t *) elem) -> tommy, elem);
}


void * rl_head (rl_t * list)
{
  return xxx_list_head (list);
}


void * rl_tail (rl_t * list)
{
  return xxx_list_tail (list);
}


void * rl_get (rl_t * list, void * elem)
{
  xxx_node * node = xxx_list_head (list);
  while (node)
    {
      if (node -> data == elem)
	return elem;
      node = node -> next;
    }
  return NULL;
}


void rl_del (rl_t * list, void * elem)
{
  if (rl_get (list, elem))
    xxx_list_remove_existing (list, & ((relem_t *) elem) -> tommy);
}
