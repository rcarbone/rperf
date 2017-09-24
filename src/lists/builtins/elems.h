#pragma once


/* Project headers */
#include "safe.h"


/* Berkeley BSD 4.4 lists */
#include <sys/queue.h>
/* undef here to avoid conflicts because LIST_HEAD is also defined in CCAN */
#undef LIST_HEAD

/* CCAN - Rusty Russell */
#include "ccan/list/list.h"

/* tommyds - Andrea Mazzoleni */
#include "tommytypes.h"

/* clibs - TJ Holowaychuk */
#include "tj-list.h"

/* ulib - Zilong Tan */
#include "ulib-list.h"

/* some implementations require fields embedded into the elements to save memory/cycles */

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

  /* Required by tommyds implementation to make the structure list-able */
  tommy_node tommy;

  /* Required by clibs implementation to make the structure list-able */
  tj_list_node_t clibs;

  /* Required by ulib implementation to make the structure list-able */
  struct ulist_head ulib;

} relem_t;


/* API */
relem_t * mkelem (unsigned key);
void rmelem (relem_t * elem);
relem_t ** mkelems (unsigned argc);
void rmelems (relem_t * argv []);
