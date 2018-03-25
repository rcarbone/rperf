/* 'rci' is an abstract specification that defines an independent C interface for Container implementations */

/*
 * Before including this header file applications need to declare their own rci_t type or
 * #define NEED_RCI_TYPEDEF to have an opaque (generic) definition
 *
 * e.g.
 *    typedef struct my_own_type rci_t;
 *    #include "rci-api.h"
 *
 * or
 *    #define NEED_RCI_TYPEDEF
 *    #include "rci-api.h"
 */


#pragma once


/* System headers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>


/* No public datatypes. The internals of the structure rci_t are all private */


#if defined(NEED_RCI_TYPEDEF)
typedef struct rci rci_t;
#endif /* NEED_RCI_TYPEDEF */


/* typedef to iterate over container nodes */
typedef void rci_each_f (void * node);


#ifdef __cplusplus
extern "C" {
#endif


/* API */

/* Allocates and initializes a container */
rci_t * rci_alloc (void);


/* Deallocates the memory used by a container */
void rci_free (rci_t * container);


/* Insert an element into a container */
void rci_put (rci_t * container, void * node);


/* Get an element from a container */
void * rci_get (rci_t * container, void * node);


/* Delete an element from a container */
void rci_del (rci_t * container, void * node);


/* Delete all elements from a container */
void rci_clear (rci_t * container);


/* Returns the size of a container */
unsigned rci_size (rci_t * container);


/* Checks if a container is empty */
bool rci_isempty (rci_t * container);


/* Iterators */
void rci_foreach (rci_t * container, rci_each_f * fn, void * data);


/* Prints container nodes inline or separating them with a new line */
void rci_print (rci_t * container);


#ifdef __cplusplus
}
#endif /* __cplusplus */
