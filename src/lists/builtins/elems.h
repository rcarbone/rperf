
#pragma once

/* Berkeley lists */
#include <sys/queue.h>


/* Element definition */
typedef struct relem relem_t;
typedef struct relem
{
  unsigned   ukey;      /* sequential */
  unsigned   umiss;

  unsigned   ukey_2;    /* dense      */
  unsigned   umiss_2;

  unsigned   ukey_3;    /* sparse     */
  unsigned   umiss_3;

  unsigned   rand_2;
  unsigned   rand_3;

  unsigned   ukbench;

  char     * skey;
  char     * smiss;
  char     * skey_2;
  char     * skey_3;

  char     * skbench;

  relem_t  * pkey;

  unsigned  uval;
  char     * sval;
  relem_t  * pval;

  /* Used to keep values in a singly-linked list of elements */
  SLIST_ENTRY (relem) head;

} relem_t;


/* API */
relem_t * mkelem (unsigned key, unsigned val, unsigned miss, unsigned dense, unsigned sparse, unsigned kbench);
void rmelem (relem_t * elem);
relem_t ** mkelems (unsigned argc);
void rmelems (relem_t * argv []);
