
#pragma once

/* Berkeley lists */
#include <sys/queue.h>


/* List Element definition */
typedef struct relem relem_t;
typedef struct relem
{
  unsigned   ukey;      /* sequential */
  unsigned   umiss;

  char     * skey;
  char     * smiss;

  relem_t  * pkey;

  unsigned  uval;
  char     * sval;
  relem_t  * pval;

  /* Used to keep values in a singly-linked list of elements */
  SLIST_ENTRY (relem) shead;

  /* Used to keep values in a double-linked list of elements */
  LIST_ENTRY (relem) head;

} relem_t;


/* API */
relem_t * mkelem (unsigned key, unsigned val, unsigned miss);
void rmelem (relem_t * elem);
relem_t ** mkelems (unsigned argc);
void rmelems (relem_t * argv []);
