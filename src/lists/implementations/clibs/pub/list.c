
//
// list.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

/*
 * Allocate a new tj_list_t. NULL on failure.
 */

tj_list_t *
list_new() {
  tj_list_t *self;
  if (!(self = LIST_MALLOC(sizeof(tj_list_t))))
    return NULL;
  self->head = NULL;
  self->tail = NULL;
  self->free = NULL;
  self->match = NULL;
  self->len = 0;
  return self;
}

/*
 * Free the list.
 */

void
list_destroy(tj_list_t *self) {
  unsigned int len = self->len;
  tj_list_node_t *next;
  tj_list_node_t *curr = self->head;

  while (len--) {
    next = curr->next;
    if (self->free) self->free(curr->val);
    LIST_FREE(curr);
    curr = next;
  }

  LIST_FREE(self);
}

/*
 * Append the given node to the list
 * and return the node, NULL on failure.
 */

tj_list_node_t *
list_rpush(tj_list_t *self, tj_list_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->prev = self->tail;
    node->next = NULL;
    self->tail->next = node;
    self->tail = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return / detach the last node in the list, or NULL.
 */

tj_list_node_t *
list_rpop(tj_list_t *self) {
  if (!self->len) return NULL;

  tj_list_node_t *node = self->tail;

  if (--self->len) {
    (self->tail = node->prev)->next = NULL;
  } else {
    self->tail = self->head = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Return / detach the first node in the list, or NULL.
 */

tj_list_node_t *
list_lpop(tj_list_t *self) {
  if (!self->len) return NULL;

  tj_list_node_t *node = self->head;

  if (--self->len) {
    (self->head = node->next)->prev = NULL;
  } else {
    self->head = self->tail = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Prepend the given node to the list
 * and return the node, NULL on failure.
 */

tj_list_node_t *
list_lpush(tj_list_t *self, tj_list_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->next = self->head;
    node->prev = NULL;
    self->head->prev = node;
    self->head = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return the node associated to val or NULL.
 */

tj_list_node_t *
list_find(tj_list_t *self, void *val) {
  list_iterator_t *it = list_iterator_new(self, TJ_LIST_HEAD);
  tj_list_node_t *node;

  while ((node = list_iterator_next(it))) {
    if (self->match) {
      if (self->match(val, node->val)) {
        list_iterator_destroy(it);
        return node;
      }
    } else {
      if (val == node->val) {
        list_iterator_destroy(it);
        return node;
      }
    }
  }

  list_iterator_destroy(it);
  return NULL;
}

/*
 * Return the node at the given index or NULL.
 */

tj_list_node_t *
list_at(tj_list_t *self, int index) {
  list_direction_t direction = TJ_LIST_HEAD;

  if (index < 0) {
    direction = TJ_LIST_TAIL;
    index = ~index;
  }

  if ((unsigned)index < self->len) {
    list_iterator_t *it = list_iterator_new(self, direction);
    tj_list_node_t *node = list_iterator_next(it);
    while (index--) node = list_iterator_next(it);
    list_iterator_destroy(it);
    return node;
  }

  return NULL;
}

/*
 * Remove the given node from the list, freeing it and it's value.
 */

void
list_remove(tj_list_t *self, tj_list_node_t *node) {
  node->prev
    ? (node->prev->next = node->next)
    : (self->head = node->next);

  node->next
    ? (node->next->prev = node->prev)
    : (self->tail = node->prev);

  if (self->free) self->free(node->val);

#if defined(ROCCO)
  LIST_FREE(node);
#endif /* ROCCO */
  --self->len;
}
