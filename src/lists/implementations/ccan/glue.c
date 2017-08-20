/* System headers */
#include <stdio.h>

/* The implementation */
#include "list/list.h"

/* librl - an abstract C library over real singly/double lists implementations */
typedef struct list_head rl_t;
#include "rl.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rl_t * rl_alloc (void)
{
  return NULL;
}


void rl_free (rl_t * list)
{

}


void rl_clear (rl_t * list)
{

}


unsigned rl_count (rl_t * list)
{
  return 0;
}


void rl_prepend (rl_t * list, relem_t * elem)
{

}


void rl_append (rl_t * list, relem_t * elem)
{

}


relem_t * rl_get (rl_t * list, relem_t * elem)
{
  return NULL;
}


void rl_del (rl_t * list, relem_t * elem)
{
}


bool rl_has (rl_t * list, relem_t * elem)
{
  return false;
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
}
