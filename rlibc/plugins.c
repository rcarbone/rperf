/* System headers */
#include <libgen.h>
#include <dirent.h>


/* Private headers */
#include "plugins.h"
#include "safe.h"
#include "varrays.h"
#include "args.h"


/* The table of mandatory names that __must__ be defined in the plugin */
static char * mandatory [] = { VARS, FUNCS, NULL };


/* Returns the content of a symbol [variable] */
static char * dlsym_variable (void * handle, char * name)
{
  char ** symbol = dlsym (handle, name);
  return symbol ? * symbol : NULL;
}


/* Returns the address of a symbol [function] */
static call_t * dlsym_function (void * handle, char * name)
{
  return dlsym (handle, name);
}


/* Make a new symbol for a [variable] */
static symbol_t * mkvariable (void * handle, char * name)
{
  symbol_t * s = NULL;
  void * sym = dlsym_variable (handle, name);
  if (sym)
    {
      s = calloc (1, sizeof (symbol_t));
      s -> name = strdup (name);
      s -> sym  = sym;
    }
  return s;
}


/* Make a new symbol for a [function] */
static symbol_t * mkfunction (void * handle, char * name)
{
  symbol_t * s = NULL;
  void * sym = dlsym_function (handle, name);
  if (sym)
    {
      s = calloc (1, sizeof (symbol_t));
      s -> name = strdup (name);
      s -> sym  = sym;
    }
  return s;
}


/* Add an item to the table of symbols */
static symbol_t ** addsymbol (symbol_t * argv [], symbol_t * s)
{
  return (symbol_t **) vamore ((void **) argv, s);
}


/* Free the table of symbols */
static void freesymbols (symbol_t * argv [])
{
  symbol_t ** s = argv;
  while (argv && * argv)
    {
      safefree ((* argv) -> name);
      free (* argv ++);
    }
  safefree (s);
}


/* Allocate and initialize a new descriptor to hold info about a plugin */
plugin_t * mkplugin (char * path, int * error, char ** argerror)
{
  void * handle;
  plugin_t * p = NULL;
  char ** names;
  char * symb;
  symbol_t * s = NULL;

  /* Load the shared object into memory */
  if (! (handle = dlopen (path, RTLD_NOW)))
    {
      * error = PLUG_NOTFOUND;
      * argerror = strdup (dlerror ());
      return NULL;
    }

  /* Time for memory */
  p = calloc (1, sizeof (plugin_t));
  p -> path     = strdup (path);
  p -> handle   = handle;
  p -> id       = strdup (basename (path));
  p -> declared = NULL;
  p -> vars     = NULL;
  p -> func     = NULL;
  p -> state    = LOADED;

  p -> id [strlen (p -> id) - SO_SUFFIX_LEN] = 0x0;

  /* Load plugin declarations. Stop if not all mandatory plugin declarations have been found */
  names = mandatory;
  symb = (char *) 1;                 /* a true value */
  while (symb && names && * names)
    p -> declared = argsmore (p -> declared, symb = dlsym_variable (handle, * names ++));

  if (! symb)
    {
      * error = PLUG_MISSINGDECL;
      * argerror = strdup (* -- names);
      rmplugin (p);
      return NULL;
    }

  /* Split declarations into arguments and load var/fun symbol tables */
  names = p -> declared;
  while (names && * names)
    {
      char ** defs = argsblanks (* names);  /* split at blanks */
      char ** d = defs;                     /* iterator */

      s = (symbol_t *) 1;                   /* a true value */
      while (s && d && * d)
	{
	  if (names == p -> declared)
	    p -> vars = addsymbol (p -> vars, s = mkvariable (handle, * d));
	  else
	    p -> func = addsymbol (p -> func, s = mkfunction (handle, * d));
	  d ++;
	}

      if (! s)
	{
	  * error = PLUG_SYMBOLNOTDEFINED;
	  * argerror = strdup (* -- d);
	  argsclear (defs);
	  rmplugin (p);
	  return NULL;
	}

      argsclear (defs);

      ++ names;
    }

  /* Bless it! */
  gettimeofday (& p -> uptime, NULL);
  p -> state = LOADED;

  return p;
}


/* Free allocated memory and resources used to hold a plugin descriptor */
plugin_t * rmplugin (plugin_t * p)
{
  if (p)
    {
      call_t * halt;
      char * argv [3] = { STOP, "now", NULL };

      /* First halt the plugin (if started) */
      if (p -> state == STARTED && (halt = function (STOP, p -> func)))
	halt (3, argv, NULL);

      safefree (p -> path);
      safefree (p -> id);

      p -> declared = argsclear (p -> declared);
      freesymbols (p -> vars);
      freesymbols (p -> func);

      if (p -> handle)
	dlclose (p -> handle);

      free (p);
    }

  return NULL;
}


/* Is 'name' a variable? */
char * variable (char * name, symbol_t * argv [])
{
  while (name && argv && * argv)
    if (! strcmp (name, (* argv) -> name))
      return (* argv) -> sym;
    else
      argv ++;
  return NULL;
}


/* Is 'name' a function? */
call_t * function (char * name, symbol_t * argv [])
{
  while (name && argv && * argv)
    if (! strcmp (name, (* argv) -> name))
      return (* argv) -> sym;
    else
      argv ++;
  return NULL;
}


/* Fill the table of candidate files checking for filenames with "*.so" extension into the directory pointed to by [dir] */
char ** rls_files (char * dir)
{
  char ** argv = NULL;
  DIR * d;

  if (dir && (d = opendir (dir)))
    {
      struct dirent * e;

      while ((e = readdir (d)))
	{
	  char * file = calloc (1, strlen (dir) + 1 + strlen (e -> d_name) + 1);
	  unsigned len;

	  sprintf (file, "%s/%s", dir, e -> d_name);
	  len = strlen (file);

	  /* Add the item to the table */
	  if (len > SO_SUFFIX_LEN && ! strncmp (& file [len - SO_SUFFIX_LEN], SO_SUFFIX, SO_SUFFIX_LEN))
	    argv = argsmore (argv, file);

	  free (file);
	}
      closedir (d);
    }
  return argssort (argv);
}


/* Get the list of plugins in the given directory */
char ** rls_plugins (char * dir)
{
  char ** shobjs = NULL;
  char ** files = rls_files (dir);
  char ** f = files;
  while (f && * f)
    {
      int err = 0;
      char * error = NULL;
      plugin_t * plugin = mkplugin (* f, & err, & error);
      if (plugin)
	{
	  shobjs = argsmore (shobjs, plugin -> path);
	  rmplugin (plugin);
	}
      else
	safefree (error);
      f ++;
    }
  argsclear (files);
  return argssort (shobjs);
}


/* Load all the plugins in the given directory */
plugin_t ** rload_plugins (char * dir, bool verbose)
{
  plugin_t ** plugins = NULL;
  char ** files = rls_files (dir);
  char ** f = files;
  while (f && * f)
    {
      int err = 0;
      char * error = NULL;
      plugin_t * p = mkplugin (* f, & err, & error);
      if (p)
	plugins = addplugin (plugins, p);
      else
	{
	  if (verbose)
	    printf ("Unable to load plugin [%s]: %d %s\n", * f, err, error);
	safefree (error);
	}
      f ++;
    }
  argsclear (files);
  return plugins;
}


/* Load only the plugins passed as arguments */
plugin_t ** rload_files (char * argv [])
{
  plugin_t ** plugins = NULL;
  char ** f = argv;
  while (f && * f)
    {
      int err;
      char * error;
      plugin_t * p = mkplugin (* f ++, & err, & error);
      if (p)
	plugins = addplugin (plugins, p);
      else
	safefree (error);
    }
  return plugins;
}


/* Unload all the plugins in the given directory */
void runload_plugins (plugin_t * argv [])
{
  plugin_t ** p = argv;
  while (p && * p)
    rmplugin (* p ++);
  safefree (argv);
}


symbol_t ** rp_getvars (plugin_t * p)
{
  return p && p -> handle && p -> vars ? p -> vars : NULL;
}


/* Check if all the plugin read-only information are accessible */
bool rp_hasvars (plugin_t * p)
{
  return rp_getvars (p);
}


/* Get plugin library */
char * rp_library (plugin_t * p)
{
  return variable ("library", rp_getvars (p));
}


/* Get plugin module */
char * rp_module (plugin_t * p)
{
  return variable ("module", rp_getvars (p));
}


/* Get plugin author */
char * rp_author (plugin_t * p)
{
  return variable ("author", rp_getvars (p));
}


/* Fill the table of plugins identifiers (module name) */
char ** rp_names (plugin_t * argv [])
{
  char ** names = NULL;
  while (argv && * argv)
    names = argsuniq (names, rp_module (* argv ++));
  return names;
}


/* Fill the table of plugins function names */
char ** rp_func_names (plugin_t * argv [])
{
  char ** names = NULL;
  while (argv && * argv)
    {
      symbol_t ** s = (* argv ++) -> func;
      while (s && * s)
	names = argsuniq (names, (* s ++) -> name);
    }
  return names;
}


/* Lookup for a module name in a table of plugins */
plugin_t * rp_lookup (char * name, plugin_t * argv [])
{
  while (name && argv && * argv)
    if (! strcmp (rp_module (* argv ++), name))
      return * -- argv;
  return NULL;
}


/* Fill the table of plugins function names */
char ** rp_functions (plugin_t * p)
{
  char ** funcs = NULL;
  symbol_t ** argv = p ? p -> func : NULL;
  while (argv && * argv)
    funcs = argsuniq (funcs, (* argv ++) -> name);
  return funcs;
}


/* Check is plugin implement a given function */
bool rp_implement (plugin_t * p, char * name)
{
  symbol_t ** argv = p ? p -> func : NULL;
  while (name && argv && * argv)
    if (! strcmp ((* argv ++) -> name, name))
      return true;
  return false;
}


/* Check at least one plugin has a given function defined */
bool rp_has (plugin_t * argv [], char * name)
{
  while (name && argv && * argv)
    if (rp_implement (* argv ++, name))
      return true;
  return false;
}


/* Counting function for better rendering */

/* Max id name length */
unsigned rp_maxi (plugin_t * argv [])
{
  unsigned n = 0;
  while (argv && * argv)
    {
      n = RMAX (n, strlen ((* argv) -> id));
      argv ++;
    }
  return n;
}


/* Max library name length */
unsigned rp_maxl (plugin_t * argv [])
{
  unsigned n = 0;
  char * s;
  while (argv && * argv)
    if ((s = rp_library (* argv ++)))
      n = RMAX (n, strlen (s));
  return n;
}


/* Max module name length */
unsigned rp_maxm (plugin_t * argv [])
{
  unsigned n = 0;
  char * s;
  while (argv && * argv)
    if ((s = rp_module (* argv ++)))
      n = RMAX (n, strlen (s));
  return n;
}


/* Max author name length */
unsigned rp_maxa (plugin_t * argv [])
{
  unsigned n = 0;
  char * s;
  while (argv && * argv)
    if ((s = rp_author (* argv ++)))
      n = RMAX (n, strlen (s));
  return n;
}


/* Rendering functions */

/* General information */
void rp_print_info (plugin_t * p, unsigned seq, unsigned maxi, unsigned maxl, unsigned maxm, unsigned maxa)
{
  if (rp_hasvars (p))
    printf ("%3d %-*.*s %-*.*s %-*.*s %-*.*s (%s)\n",
	    seq,
	    maxi, maxi, p -> id,
	    maxl, maxl, rp_library (p),
	    maxm, maxm, rp_module (p),
	    maxa, maxa, rp_author (p),
	    basename (p -> path));
}


/* Unique identifier in terms of Library / Module / Author */
void rp_print_id (plugin_t * p, unsigned seq, unsigned maxl, unsigned maxm, unsigned maxa)
{
  if (rp_hasvars (p))
    printf ("%3d %-*.*s %-*.*s %-*.*s (%s)\n", seq,
	    maxl, maxl, rp_library (p),
	    maxm, maxm, rp_module (p),
	    maxa, maxa, rp_author (p),
	    basename (p -> path));
}


/* General information */
void rp_print_infos (plugin_t * argv [])
{
  unsigned maxi = rp_maxi (argv);
  unsigned maxl = rp_maxl (argv);
  unsigned maxm = rp_maxm (argv);
  unsigned maxa = rp_maxa (argv);
  unsigned seq = 0;

  /* Table Header */
  if (argv)
    printf (" ## %-*.*s %-*.*s %-*.*s %-*.*s %s\n",
	    maxi, maxi, "Id",
	    maxl, maxl, "Library",
	    maxm, maxm, "Module",
	    maxa, maxa, "Author",
	    "Filename");

  while (argv && * argv)
    rp_print_info (* argv ++, ++ seq, maxi, maxl, maxm, maxa);
}


void rp_print_ids (plugin_t * argv [])
{
  unsigned maxl = rp_maxl (argv);
  unsigned maxm = rp_maxm (argv);
  unsigned maxa = rp_maxa (argv);
  unsigned seq = 0;

  /* Table Header */
  if (argv)
    printf (" ## %-*.*s %-*.*s %-*.*s %s\n", maxl, maxl, "Library", maxm, maxm, "Module", maxa, maxa, "Author", "Filename");

  while (argv && * argv)
    rp_print_id (* argv ++, ++ seq, maxl, maxm, maxa);
}
