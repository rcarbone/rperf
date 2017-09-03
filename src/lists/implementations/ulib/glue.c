/* System headers */
#include <stdlib.h>


/* The implementation */
#include "ulib-list.h"


/* Project headers */
#include "elems.h"
#include "safe.h"

/* librl - an abstract C library over real list implementations */
typedef struct ulist_head rl_t;
#include "rl-api.h"


rl_t * rl_alloc (void)
{
  rl_t * list = calloc (1, sizeof (* list));
  INIT_LIST_HEAD (list);
  return list;
}


void rl_free (rl_t * list)
{
  safefree (list);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  relem_t * elem;
  ulist_for_each_entry (elem, list, ulib)
    if (fn)
      fn (data);
}


unsigned rl_count (rl_t * list)
{
  unsigned count = 0;
  relem_t * elem;
  ulist_for_each_entry (elem, list, ulib)
    count ++;
  return count;
}


void rl_prepend (rl_t * list, void * elem)
{
  ulist_add (& ((relem_t *) elem) -> ulib, list);
}


void rl_append (rl_t * list, void * elem)
{
  ulist_add_tail (& ((relem_t *) elem) -> ulib, list);
}


void * rl_head (rl_t * list)
{
  return ulist_first_entry (list, relem_t, ulib);
}


void * rl_tail (rl_t * list)
{
  relem_t * elem;
  ulist_for_each_entry (elem, list, ulib)
    if (ulist_is_last (& elem -> ulib, list))
      return elem;
  return NULL;
}


void * rl_get (rl_t * list, void * elem)
{
  relem_t * item;
  ulist_for_each_entry (item, list, ulib)
    if (item == elem)
      return item;
  return NULL;
}


void rl_del (rl_t * list, void * elem)
{
  relem_t * item;
  ulist_for_each_entry (item, list, ulib)
    if (item == elem)
      {
	ulist_del (& item -> ulib);
	break;
      }
}
