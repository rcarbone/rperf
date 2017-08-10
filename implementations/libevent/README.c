
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Plugin declarations */
const char * variables   = "library module author description url";
const char * functions   =
  "boot halt grow_seq grow_rnd hit_seq hit_rnd miss_seq miss_rnd delete_seq delete_rnd replace_seq kbench";

/* replace_rnd is too slow (may be undefinite loop) */

/* Plugin definitions */
const char * library     = "libevent";
const char * module      = "libevent/ht";
const char * author      = "Niels Provos (provos@umich.edu)";
const char * description = "Niels Provos's interface to Christopher Clark's implementation";
const char * url         = "http://github.com/libevent/libevent";

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
