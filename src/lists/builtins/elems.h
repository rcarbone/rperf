
#pragma once

/* Berkeley lists */
#include <sys/queue.h>


/* List Element definition */
typedef struct relem relem_t;
typedef struct relem
{
  unsigned   foo;
  char     * bar;
  relem_t  * me;

  /* Used to keep values in a singly-linked list of elements */
  SLIST_ENTRY (relem) shead;

  /* Used to keep values in a double-linked list of elements */
  LIST_ENTRY (relem) head;

  /* Used to keep values in a singly-linked tail queue of elements */
  STAILQ_ENTRY (relem) stailq;

  /* Used to keep values in a doubly-linked tail queue of elements */
  TAILQ_ENTRY (relem) tailq;

} relem_t;


/* API */
relem_t * mkelem (unsigned key);
void rmelem (relem_t * elem);
relem_t ** mkelems (unsigned argc);
void rmelems (relem_t * argv []);
