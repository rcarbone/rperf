/* System headers */
#include <stdlib.h>


/* The implementation */
#include "klist.h"


/* Project headers */
#include "elems.h"
#include "safe.h"

/* librl - an abstract C library over real list implementations */
#define nofree(x)
KLIST_INIT(kl, relem_t *, nofree)
typedef klist_t (kl) rl_t;
#include "rl-api.h"


rl_t * rl_alloc (void)
{
  return kl_init (kl);
}


void rl_free (rl_t * list)
{
  kl_destroy (kl, list);
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  kliter_t (kl) * it;
  for (it = kl_begin (list); it != kl_end (list); it = kl_next (it))
    if (fn)
      fn (data);
}


unsigned rl_count (rl_t * list)
{
  return list -> size;
}


void rl_prepend (rl_t * list, void * elem)
{
#if defined(ROCCO)
  kl1_kl * p = kmp_alloc (kl, list -> mp);

  p -> data = elem;
  p -> next = list -> head;
  list -> head = p;

  ++ list -> size;
#else
  * kl_pushp (kl, list) = elem;
#endif /* ROCCO */
}


void rl_append (rl_t * list, void * elem)
{
  * kl_pushp (kl, list) = elem;
}


void * rl_head (rl_t * list)
{
  return kl_begin (list);
}


void * rl_tail (rl_t * list)
{
  return kl_end (list);
}


void * rl_get (rl_t * list, void * elem)
{
  kliter_t (kl) * it;
  for (it = kl_begin (list); it != kl_end (list); it = kl_next (it))
    if (kl_val (it) == elem)
      return elem;
  return NULL;
}


void rl_del (rl_t * list, void * elem)
{
  kliter_t (kl) * it;
  for (it = kl_begin (list); it != kl_end (list); it = kl_next (it))
    if (kl_val (it) == elem)
      kl_shift (kl, list, NULL);
}
