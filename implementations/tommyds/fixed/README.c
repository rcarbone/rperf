
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Plugin declarations */
const char * variables   = "library module author description url";
const char * functions   =
  "boot halt grow_seq grow_rnd hit_seq hit_rnd miss_seq miss_rnd delete_seq delete_rnd replace_seq";

/* replace_rnd fails (need to be investigated) */
/* kbench is too slow */

/* Plugin definitions */
const char * library     = "tommyds";
const char * module      = "tommyds/fixed";
const char * author      = "Andrea Mazzoleni (amadvance@gmail.com)";
const char * description = "Fixed size chained hashtable";
const char * url         = "http://github.com/amadvance/tommyds";

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
