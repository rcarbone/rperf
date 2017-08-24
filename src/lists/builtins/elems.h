
#pragma once

/* Berkeley 4.4 lists */
#include <sys/queue.h>
#undef LIST_HEAD

/* CCAN - Rusty Russell */
#include "ccan/list/list.h"


/* List Element definition */
typedef struct relem relem_t;
typedef struct relem
{
  unsigned   foo;
  char     * bar;
  relem_t  * me;

  /* Required by BSD implementation to keep values in a singly-linked list of elements */
  SLIST_ENTRY (relem) shead;

  /* Required by BSD implementation to keep values in a double-linked list of elements */
  LIST_ENTRY (relem) head;

  /* Required by BSD implementation to keep values in a singly-linked tail queue of elements */
  STAILQ_ENTRY (relem) stailq;

  /* Required by BSD implementation to keep values in a doubly-linked tail queue of elements */
  TAILQ_ENTRY (relem) tailq;

  /* Required by CCAN implementation to make the structure list-able */
  struct list_node ccan;

} relem_t;


/* API */
relem_t * mkelem (unsigned key);
void rmelem (relem_t * elem);
relem_t ** mkelems (unsigned argc);
void rmelems (relem_t * argv []);
