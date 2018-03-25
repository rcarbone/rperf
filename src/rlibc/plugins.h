/*
 * The definitions to load/unload/query/call plugins.
 *
 * What is a plugin for the goal of the project?
 *  A plugin is simply a shared object with basic restrictions to distinguish it from other common Unix shated object files:
 */

#pragma once


/* System headers */
#include <stdbool.h>
#include <sys/time.h>
#include <dlfcn.h>


/* Project headers */
#include "sargv.h"


/* Reserved keywords with mandatory names that _must_ be defined in each plugin in a blanks separated variable */
#define RPLUGIN_VARS    "variables"
#define RPLUGIN_FUNCS   "functions"


/* Reserved keywords to initialize and terminate the plugin */
#define RPLUGIN_START   "boot"
#define RPLUGIN_STOP    "halt"


/* Reasons rplugin_load() could fail */
#define RPLUGIN_NOTFOUND         100  /* [Warning] - Plugin cannot be found                          */
#define RPLUGIN_MISSINGDECL      200  /* [Warning] - Missing or illegal mandatory plugin declaration */
#define RPLUGIN_SYMBOLNOTDEFINED 300  /* [Warning] - Symbol declared but not defined                 */

/* Default plugin directory */
#if !defined(RPLUGIN_DEFAULT_DIR)
#define RPLUGIN_DEFAULT_DIR   "../plugins"
#endif

/* Plugin files candidates to be loaded must have the following suffix */
#define RPLUGIN_SO_SUFFIX      ".so"
#define RPLUGIN_SO_SUFFIX_LEN  3


/* Add/Clear plugin items to a table */
#define rplugin_more(argv, item) (rplugin_t **) vamore ((void **) argv, (void *) item)
#define rplugin_clear(argv)      (rplugin_t **) vaclear ((void **) argv, rmplugin)


/* The prototype for the a generic function that could be implemented in the plugin */
typedef int rplugin_f (int argc, char * argv [], void * envp []);


/* The prototype for the a generic symbol that could be implemented in the plugin */
typedef struct
{
  char * name;             /* User printable variable/function name */
  void * sym;              /* Its handle as obtained by dlsym()     */

} rplugin_symbol_t;


/* Keeps all information about the plugin implemented in a shared object */
typedef struct
{
  struct timeval uptime;     /* Time it was loaded                 */

  char      * path;          /* Where the shared object is located */
  void      * handle;        /* Handle returned by dlopen()        */
  char      * id;            /* unique id                          */

  char     ** declared;      /* Table of plugin declarations       */

  rplugin_symbol_t ** vars;  /* Table of variables implemented     */
  rplugin_symbol_t ** funs;  /* Table of functions implemented     */

  enum
    { RPLUGIN_LOADED, RPLUGIN_STARTED }
    state;                   /* State of the plugin                */

} rplugin_t;


/* -=-=-=-=-=-=-= API -=-=-=-=-=-=-= */

/* Allocate and free dynamic memory to hold a plugin */
rplugin_t * rplugin_mk (char * path, int * code, char ** error);
rplugin_t * rplugin_rm (rplugin_t * plugin);

/* Load/unload in memory plugin(s) passed by filename(s) */
rplugin_t * rplugin_open (char * file);
void rplugin_close (rplugin_t * plugin);
rplugin_t ** rplugin_load_files (char * files []);
rplugin_t ** rplugin_load_dir (char * dir, bool verbose);
void rplugin_unload (rplugin_t * argv []);

/* List/Load plugins in a given directory */
char ** rplugin_ls (char * dir);

/* Check is a name has been defined in the table of symbols */
char * rplugin_variable (char * name, rplugin_symbol_t * argv []);
rplugin_f * rplugin_function (char * name, rplugin_symbol_t * argv []);
bool rplugin_defined (char * name, rplugin_symbol_t * argv []);

/* Get functions (single plugin) */
rplugin_symbol_t ** rplugin_getvars (rplugin_t * plugin);
bool rplugin_hasvars (rplugin_t * plugin);

char * rplugin_library (rplugin_t * plugin);
char * rplugin_module (rplugin_t * plugin);
char * rplugin_author (rplugin_t * plugin);

char ** rplugin_names (rplugin_t * argv []);
char ** rplugin_func_names (rplugin_t * argv []);
char ** rplugin_functions (rplugin_t * plugin);
bool rplugin_implement (rplugin_t * plugin, char * name);
bool rplugin_has (rplugin_t * argv [], char * name);
rplugin_t * rplugin_lookup (char * name, rplugin_t * argv []);

unsigned rplugin_maxi (rplugin_t * argv []);
unsigned rplugin_maxl (rplugin_t * argv []);
unsigned rplugin_maxm (rplugin_t * argv []);
unsigned rplugin_maxa (rplugin_t * argv []);

void rplugin_print_info (rplugin_t * plugin, unsigned seq, unsigned maxi, unsigned maxl, unsigned maxm, unsigned maxa);
void rplugin_print_id (rplugin_t * plugin, unsigned seq, unsigned maxl, unsigned maxm, unsigned maxa);

void rplugin_print_infos (rplugin_t * argv []);
void rplugin_print_ids (rplugin_t * argv []);
