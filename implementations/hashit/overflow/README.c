
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* Plugin declarations */
const char * variables   = "library module author description url";
const char * functions   =
  "boot halt grow_seq grow_rnd hit_seq hit_rnd miss_seq miss_rnd delete_seq delete_rnd replace_seq";

/* replace_rnd kbench cannot be run due to SIGSEGV */

/* Plugin definitions */
const char * library     = "hashit";
const char * module      = "hashit/overflow";
const char * author      = "David Gomez (david@pleyades.net)";
const char * description = "Hashit - Another generic hash library";
const char * url         = "http://freecode.com/projects/hashit";

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
