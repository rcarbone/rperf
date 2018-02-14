#pragma once


/* Definition for a function implementing an Unit Test */
typedef unsigned rut_f (unsigned argc);


/* The structure to keep Unit Tests */
typedef struct
{
  char * name;            /* unique name               */
  char * description;     /* description               */
  rut_f * func;           /* function to run Unit Test */

} rut_t;



#ifdef __cplusplus
extern "C" {
#endif

unsigned rutargc (void);
rut_t * rutargv (void);


/* Returns Unit Test handles */
rut_t * rut_first (rut_t * argv);
rut_t * rut_last (unsigned argc, rut_t * argv);
rut_t * rut_at (unsigned argc, rut_t * argv, unsigned at);
rut_t * rut_rnd (unsigned argc, rut_t * argv);
rut_t * rut_name (unsigned argc, rut_t * argv, char * name);
rut_t * rut_valid (unsigned argc, rut_t * argv, char * id);
unsigned rut_maxn (unsigned argc, rut_t * argv);
void rut_print (rut_t * ut, unsigned indent, unsigned maxn);
void rut_run (rut_t * unit, unsigned items, unsigned n, unsigned seq, unsigned maxn);

rut_t ** rut_handles (unsigned argc, rut_t * argv);
rut_t ** rut_shuffle (unsigned argc, rut_t * argv);
char ** rut_names (unsigned argc, rut_t * argv);

/* Print all the Unit Tests */
void rut_prints (unsigned argc, rut_t * argv);


#ifdef __cplusplus
}
#endif /* __cplusplus */
