
//
// list.h
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#ifndef LIST_H
#define LIST_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

// Library version

#define LIST_VERSION "0.0.5"

// Memory management macros

#ifndef LIST_MALLOC
#define LIST_MALLOC malloc
#endif

#ifndef LIST_FREE
#define LIST_FREE free
#endif

/*
 * tj_list_t iterator direction.
 */

typedef enum {
    TJ_LIST_HEAD
  , TJ_LIST_TAIL
} list_direction_t;

/*
 * tj_list_t node struct.
 */

typedef struct tj_list_node {
  struct tj_list_node *prev;
  struct tj_list_node *next;
  void *val;
} tj_list_node_t;

/*
 * tj_list_t struct.
 */

typedef struct {
  tj_list_node_t *head;
  tj_list_node_t *tail;
  unsigned int len;
  void (*free)(void *val);
  int (*match)(void *a, void *b);
} tj_list_t;

/*
 * tj_list_t iterator struct.
 */

typedef struct {
  tj_list_node_t *next;
  list_direction_t direction;
} list_iterator_t;

// Node prototypes.

tj_list_node_t *
list_node_new(void *val);

// tj_list_t prototypes.

tj_list_t *
list_new();

tj_list_node_t *
list_rpush(tj_list_t *self, tj_list_node_t *node);

tj_list_node_t *
list_lpush(tj_list_t *self, tj_list_node_t *node);

tj_list_node_t *
list_find(tj_list_t *self, void *val);

tj_list_node_t *
list_at(tj_list_t *self, int index);

tj_list_node_t *
list_rpop(tj_list_t *self);

tj_list_node_t *
list_lpop(tj_list_t *self);

void
list_remove(tj_list_t *self, tj_list_node_t *node);

void
list_destroy(tj_list_t *self);

// tj_list_t iterator prototypes.

list_iterator_t *
list_iterator_new(tj_list_t *list, list_direction_t direction);

list_iterator_t *
list_iterator_new_from_node(tj_list_node_t *node, list_direction_t direction);

tj_list_node_t *
list_iterator_next(list_iterator_t *self);

void
list_iterator_destroy(list_iterator_t *self);

#ifdef __cplusplus
}
#endif

#endif /* LIST_H */
