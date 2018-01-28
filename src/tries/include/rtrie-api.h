/* 'rtrie' is an abstract specification that defines an independent C interface for Trie implementations */

/*
 * Before including this header file applications need to declare their own rtrie_t type or
 * #define NEED_RTRIE_TYPEDEF to have an opaque (generic) definition
 *
 * e.g.
 *    typedef struct my_own_trie rtrie_t;
 *    #include "rtrie-api.h"
 *
 * or
 *    #define NEED_RTRIE_TYPEDEF
 *    #include "rtrie-api.h"
 */


#pragma once


/* System headers */
#include <stdio.h>
#include <stdbool.h>


/* No public datatypes. The internals of the structure rtrie_t are all private */


#if defined(NEED_RTRIE_TYPEDEF)
typedef struct rtrie rtrie_t;
#endif /* NEED_RTRIE_TYPEDEF */


/* typedef to iterate over trie nodes */
typedef void rtrie_each_f (void * node);


#ifdef __cplusplus
extern "C" {
#endif


/* API */

/* Allocates and initializes the trie */
rtrie_t * rtrie_alloc (void);

/* Deallocates the memory used by the trie */
void rtrie_free (rtrie_t * trie);


/* Iterators */
void rtrie_foreach (rtrie_t * trie, rtrie_each_f * fn, void * data);


/* Returns the size of a trie */
unsigned rtrie_count (rtrie_t * trie);


/* Checks if trie is empty */
bool rtrie_isempty (rtrie_t * trie);


/* Write */
void rtrie_add (rtrie_t * trie, void * node);

/* Read */
void * rtrie_get (rtrie_t * trie, void * node);

/* Delete */
void rtrie_del (rtrie_t * trie, void * node);


/* Prints trie nodes inline or separating them with a new line */
void rtrie_print (rtrie_t * trie);


#ifdef __cplusplus
}
#endif /* __cplusplus */
