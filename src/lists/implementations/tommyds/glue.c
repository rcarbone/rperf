/* System headers */
#include <stdlib.h>


/* The implementation */
#include "tommylist.c"


/* Project headers */

/* librl - an abstract C library over real singly/double lists implementations */
typedef tommy_list rl_t;
#include "rl.h"

#include "elems.h"
#include "safe.h"


rl_t * rl_alloc (void)
{
  rl_t * list = calloc (1, sizeof (* list));
  tommy_list_init (list);
  return list;
}


void rl_free (rl_t * list)
{
  safefree (list);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  tommy_node * node = tommy_list_head (list);
  while (node)
    {
      if (fn)
	fn (data);
      node = node -> next;
    }
}


unsigned rl_count (rl_t * list)
{
  return tommy_list_count (list);
}


void rl_prepend (rl_t * list, void * elem)
{
  tommy_list_insert_head (list, & ((relem_t *) elem) -> tommy, elem);
}


void rl_append (rl_t * list, void * elem)
{
  tommy_list_insert_tail (list, & ((relem_t *) elem) -> tommy, elem);
}


void * rl_get (rl_t * list, void * elem)
{
  tommy_node * node = tommy_list_head (list);
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
    tommy_list_remove_existing (list, & ((relem_t *) elem) -> tommy);
}
