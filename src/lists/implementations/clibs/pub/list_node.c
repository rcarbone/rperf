
//
// node.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

/*
 * Allocates a new tj_list_node_t. NULL on failure.
 */

tj_list_node_t *
list_node_new(void *val) {
  tj_list_node_t *self;
  if (!(self = LIST_MALLOC(sizeof(tj_list_node_t))))
    return NULL;
  self->prev = NULL;
  self->next = NULL;
  self->val = val;
  return self;
}
