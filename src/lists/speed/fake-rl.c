/* System headers */
#include <stdio.h>
#include <stdlib.h>


/* Project headers */
#include "safe.h"


/* librl - an abstract C library over real list implementations */
#define NEED_RL_TYPEDEF
#include "rl.h"


typedef struct rl
{
  unsigned count;

} rl_t;


static void never (char * func)
{
  printf ("The function [%s] is only for reference and it should never be called\n", func);
  fflush (stdout);
}


rl_t * rl_alloc (void)
{
  never ("rl_alloc");
  return calloc (1, sizeof (rl_t));
}


void rl_free (rl_t * list)
{
  never ("rl_free");
  safefree (list);
}


void rl_clear (rl_t * list)
{
  never ("rl_clear");
}


unsigned rl_count (rl_t * list)
{
  never ("rl_count");
  return list ? list -> count : 0;
}


void rl_prepend (rl_t * list, relem_t * elem)
{
  never ("rl_prepend");
}


void rl_append (rl_t * list, relem_t * elem)
{
  never ("rl_append");
}


relem_t * rl_get (rl_t * list, relem_t * elem)
{
  never ("rl_get");
  return NULL;
}


void rl_del (rl_t * list, relem_t * elem)
{
  never ("rl_del");
}


bool rl_has (rl_t * list, relem_t * elem)
{
  never ("rl_has");
  return false;
}


void rl_foreach (rl_t * list, rl_each_f * fn, void * data)
{
  never ("rl_foreach");
}
