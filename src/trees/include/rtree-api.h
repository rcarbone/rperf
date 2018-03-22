/* 'rtree' is an abstract specification that defines an independent C interface for Trie implementations */

/*
 * Before including this header file applications need to declare their own rtree_t type or
 * #define NEED_RTREE_TYPEDEF to have an opaque (generic) definition
 *
 * e.g.
 *    typedef struct my_own_trie rtree_t;
 *    #include "rtree-api.h"
 *
 * or
 *    #define NEED_RTREE_TYPEDEF
 *    #include "rtree-api.h"
 */


#pragma once


/* System headers */
#include <stdio.h>
#include <stdbool.h>


/* No public datatypes. The internals of the structure rtree_t are all private */


#if defined(NEED_RTREE_TYPEDEF)
typedef struct rtree rtree_t;
#endif /* NEED_RTREE_TYPEDEF */


/* typedef to iterate over trie nodes */
typedef void rtree_each_f (void * node);


#ifdef __cplusplus
extern "C" {
#endif


/* API */

/* Allocates and initializes the trie */
rtree_t * rtree_alloc (void);


/* Deallocates the memory used by the trie */
void rtree_free (rtree_t * trie);


/* Returns the size of a trie */
unsigned rtree_count (rtree_t * trie);


/* Checks if trie is empty */
bool rtree_isempty (rtree_t * trie);


/* Write */
void rtree_add (rtree_t * trie, void * node);


/* Read */
void * rtree_get (rtree_t * trie, void * node);


/* Delete */
void rtree_del (rtree_t * trie, void * node);


/* Iterators */
void rtree_foreach (rtree_t * trie, rtree_each_f * fn, void * data);


/* Prints trie nodes inline or separating them with a new line */
void rtree_print (rtree_t * trie);


#ifdef __cplusplus
}
#endif /* __cplusplus */
