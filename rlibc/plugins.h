/* The definition to load/unload/query/call plugins */

#pragma once

/* System headers */
#include <stdbool.h>
#include <sys/time.h>
#include <dlfcn.h>


/* Reserved keywords with mandatory names that _must_ be defined in each plugin in a blanks separated variable */
#define VARS     "variables"
#define FUNCS    "functions"


/* Reserved keywords to initialize and terminate the plugin */
#define START     "boot"
#define STOP      "halt"


/* Reasons load_plugin() could fail */
#define PLUG_NOTFOUND         100  /* [Warning] - Plugin cannot be found                               */
#define PLUG_NOMEMORYPLUG     200  /* [Error]   - Not enough memory to allocate plugin                 */
#define PLUG_MISSINGDECL      300  /* [Warning] - Missing or illegal mandatory plugin declaration      */
#define PLUG_SYMBOLNOTDEFINED 400  /* [Warning] - Symbol declared but not defined                      */
#define PLUG_ALREADYLOADED    500  /* [Warning] - Plugin is already loaded                             */
#define PLUG_NOMEMORYTABLE    600  /* [Error]   - Not enough memory to reallocate the table of plugins */
#define PLUG_NOSTART          700  /* [Warning] - Plugin cannot be started!                            */


/* Default plugin directory */
#define DEFAULT_DIR    "../plugins"

/* Files candidates to be loaded must have the following suffix */
#define SO_SUFFIX      ".so"
#define SO_SUFFIX_LEN  3


#define addplugin(argv, item) (plugin_t **) vamore ((void **) argv, (void *) item)
#define freeplugins(argv)     (plugin_t **) vaclear ((void **) argv, rmplugin)


/* The prototype for the a generic function that could be implemented by the plugin */
typedef int call_t (int argc, char * argv [], void * envp []);


/* The prototype for the a generic symbol that could be implemented in the plugin */
typedef struct
{
  char * name;             /* User printable variable/function name */
  void * sym;              /* Its handle as obtained by dlsym()     */

} symbol_t;


/* Keeps all information about the plugin implemented in a shared object */
typedef struct plugin_t
{
  struct timeval uptime;   /* Time it was loaded                 */

  char      * path;        /* Where the shared object is located */
  void      * handle;      /* Handle returned by dlopen()        */
  char      * id;          /* unique id                          */

  char     ** declared;    /* Table of plugin declarations       */

  symbol_t ** vars;        /* Table of variables implemented     */
  symbol_t ** func;        /* Table of functions implemented     */

  enum
    { LOADED, STARTED }
    state;                 /* State of the plugin                */

} plugin_t;



/* -=-=-=-=-=-=-= API -=-=-=-=-=-=-= */

#define PLUGIN_OK    0
#define PLUGIN_FAIL  1

plugin_t * mkplugin (char * path, int * error, char ** argerror);
plugin_t * rmplugin (plugin_t * p);

void * defined (char * name, symbol_t * argv []);
char * variable (char * name, symbol_t * argv []);
call_t * function (char * name, symbol_t * argv []);

char * pluginname (plugin_t * p);
plugin_t * pluginbyname (char * name);
int pluginstate (char * name);


/* List plugins in a given directory */
char ** rls_plugins (char * dir);

/* Load plugins in a given directory */
plugin_t ** rload_plugins (char * dir, bool verbose);

/* Load plugins passed by filenames */
plugin_t ** rload_files (char * files []);

/* Unload plugins */
void runload_plugins (plugin_t * plugins []);


/* Get functions */
symbol_t ** rp_getvars (plugin_t * p);
bool rp_hasvars (plugin_t * p);

char * rp_library (plugin_t * p);
char * rp_module (plugin_t * p);
char * rp_author (plugin_t * p);

char ** rp_names (plugin_t * p []);
char ** rp_func_names (plugin_t * argv []);
plugin_t * rp_lookup (char * name, plugin_t * p []);
char ** rp_functions (plugin_t * p);
bool rp_implement (plugin_t * p, char * name);
bool rp_has (plugin_t * argv [], char * name);

unsigned rp_maxi (plugin_t * argv []);
unsigned rp_maxl (plugin_t * argv []);
unsigned rp_maxm (plugin_t * argv []);
unsigned rp_maxa (plugin_t * argv []);

void rp_print_info (plugin_t * p, unsigned seq, unsigned maxi, unsigned maxl, unsigned maxm, unsigned maxa);
void rp_print_id (plugin_t * p, unsigned seq, unsigned maxl, unsigned maxm, unsigned maxa);

void rp_print_infos (plugin_t * argv []);
void rp_print_ids (plugin_t * argv []);
