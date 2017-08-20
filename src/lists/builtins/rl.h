/* The 'rl' is an abstract specification that defines an independent C interface for List implementations */

/*
 * Before including this header file applications need to declare their own rl_t type or
 * define NEED_RL_TYPEDEF to have a generic definition
 *
 * e.g.
 *  typedef struct my_own_list rl_t;
 *  #include "rl.h"
 *
 * or
 * #define NEED_RL_TYPEDEF
 *  #include "rl.h"
 */

#pragma once

#if defined(NEED_RL_TYPEDEF)
typedef struct rl rl_t;
#endif /* NEED_RL_TYPEDEF */


/* System headers */
#include <stdio.h>
#include <stdbool.h>


/* Project headers */
#include "elems.h"


/* No public datatypes. The internals of the structure rl_t are all private */


/* typedef to iterate over list elements */
typedef void rl_each_f (void * elem);


/* API */

rl_t * rl_alloc (void);
void rl_free (rl_t * list);
void rl_clear (rl_t * list);
unsigned rl_count (rl_t * list);
void rl_prepend (rl_t * list, relem_t * elem);
void rl_append (rl_t * list, relem_t * elem);
relem_t * rl_get (rl_t * list, relem_t * elem);
void rl_del (rl_t * list, relem_t * elem);
bool rl_has (rl_t * list, relem_t * elem);
void rl_foreach (rl_t * list, rl_each_f * fn, void * data);
