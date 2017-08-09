#pragma once

/* Project headers */
#include "rtypes.h"
#include "datasets.h"


/* The local identifiers for the Test Suite to run (indexed by keys) */
typedef enum
{
  RSUITE_GROW_SEQ = 0x01,  /* Populate an empty container using sequential generated keys */
  RSUITE_GROW_RND = 0x02,  /* Populate an empty container using random generated keys     */
  RSUITE_HIT_SEQ  = 0x03,  /* Populate and search all existing items one-by-one  */
  RSUITE_HIT_RND  = 0x04,  /* Populate and search all existing items one-by-one  */
  RSUITE_MISS_SEQ = 0x05,  /* Populate and search for non existing items         */
  RSUITE_MISS_RND = 0x06,  /* Populate and search for non existing items         */
  RSUITE_DELETE   = 0x07,  /* Populate and delete all existing items one-by-one  */
  RSUITE_REPLACE  = 0x08,  /* Populate and replace all existing items one-by-one */
  RSUITE_KBENCH   = 0x09,  /* Delete if found, add otherwise items one-by-one    */

} rsuite_id_t;


/* The implementations elsewhere defined */
rsuite_f rsuite_grow_seq;
rsuite_f rsuite_grow_rnd;
rsuite_f rsuite_hit_seq;
rsuite_f rsuite_hit_rnd;
rsuite_f rsuite_miss_seq;
rsuite_f rsuite_miss_rnd;
rsuite_f rsuite_delete;
rsuite_f rsuite_replace;
rsuite_f rsuite_kbench;

