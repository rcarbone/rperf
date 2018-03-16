#pragma once

/* Project headers */
#include "rtypes.h"
#include "rtest.h"


/* Definition for a function implementing a Suite */
typedef rtime_t rsuite_f (unsigned argc, void * argv []);


/* The structure to keep a Suite */
typedef struct
{
  char * name;            /* unique name               */
  char * description;     /* description               */
  rsuite_f * func;        /* function to run the Suite */

  relapsed_t ** results;  /* results of test execution */

} rsuite_t;



/* Returns # of builtins items */
unsigned rsuiteargc (void);
rsuite_t * rsuiteargv (void);

/* Returns Suite handles */
rsuite_t * rsuite_first (rsuite_t * argv);
rsuite_t * rsuite_last (unsigned argc, rsuite_t * argv);
rsuite_t * rsuite_at (unsigned argc, rsuite_t * argv, unsigned at);
rsuite_t * rsuite_rnd (unsigned argc, rsuite_t * argv);
rsuite_t * rsuite_name (unsigned argc, rsuite_t * argv, char * name);
rsuite_t * rsuite_valid (unsigned argc, rsuite_t * argv, char * id);
unsigned rsuite_maxn (unsigned argc, rsuite_t * argv);
unsigned rsuite_maxd (unsigned argc, rsuite_t * argv);
void rsuite_print (rsuite_t * suite, unsigned n, unsigned maxn);
void rsuite_run (rsuite_t * suite, unsigned argc, void * argv, unsigned digits, unsigned seq, unsigned maxn);

rsuite_t ** rsuite_handles (unsigned argc, rsuite_t * argv);
rsuite_t ** rsuite_shuffle (unsigned argc, rsuite_t * argv);
char ** rsuite_names (unsigned argc, rsuite_t * argv);
void rsuite_prints (unsigned argc, rsuite_t * argv);

void rsuite_clear_results (rsuite_t * suite []);
void rsuite_sort_results (rsuite_t * suite []);

unsigned rsuite_MAXN (rsuite_t * suite []);
unsigned rsuite_MAXD (rsuite_t * suite []);
char ** rsuite_NAMES (rsuite_t * suite []);


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */
unsigned rtrie_suite_argc (void);
rsuite_t * rtrie_suite_argv (void);
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* declarations */
rsuite_f rsuite_grow_seq;
rsuite_f rsuite_grow_rnd;
rsuite_f rsuite_hit_seq;
rsuite_f rsuite_hit_rnd;
rsuite_f rsuite_pop;
rsuite_f rsuite_iterate;
