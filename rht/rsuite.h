#pragma once

/* Project headers */
#include "rtypes.h"
#include "datasets.h"


/* The local identifiers for the Test Suite to run */
typedef enum
{
  RSUITE_GROW_SEQ    = 0x01,  /* Populate an empty container using sequential generated keys                        */
  RSUITE_GROW_RND    = 0x02,  /* Populate an empty container using random generated keys                            */
  RSUITE_HIT_SEQ     = 0x03,  /* Populate and search all existing items one-by-one using sequential generated keys  */
  RSUITE_HIT_RND     = 0x04,  /* Populate and search all existing items one-by-one  */
  RSUITE_MISS_SEQ    = 0x05,  /* Populate and search for non existing items using sequential generated keys */
  RSUITE_MISS_RND    = 0x06,  /* Populate and search for non existing items         */
  RSUITE_DELETE_SEQ  = 0x07,  /* Populate and delete all existing items one-by-one using sequential generated keys  */
  RSUITE_DELETE_RND  = 0x08,  /* Populate and delete all existing items one-by-one  */
  RSUITE_REPLACE_SEQ = 0x09,  /* Populate and replace all existing items one-by-one using sequential generated keys */
  RSUITE_REPLACE_RND = 0x0a,  /* Populate and replace all existing items one-by-one */
  RSUITE_KBENCH      = 0x0b,  /* Delete if found, add otherwise items (non unique keys) */

} rsuite_id_t;


/* The implementations elsewhere defined */
rsuite_f rsuite_grow_seq;
rsuite_f rsuite_grow_rnd;
rsuite_f rsuite_hit_seq;
rsuite_f rsuite_hit_rnd;
rsuite_f rsuite_miss_seq;
rsuite_f rsuite_miss_rnd;
rsuite_f rsuite_delete_seq;
rsuite_f rsuite_delete_rnd;
rsuite_f rsuite_replace_seq;
rsuite_f rsuite_replace_rnd;
rsuite_f rsuite_kbench;

