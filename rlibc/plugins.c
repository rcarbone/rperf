/* System headers */
#include <libgen.h>
#include <dirent.h>


/* Private headers */
#include "plugins.h"
#include "safe.h"
#include "varrays.h"
#include "args.h"


/* The table of mandatory names that __must__ be defined in the plugin */
static char * mandatory [] = { RPLUGIN_VARS, RPLUGIN_FUNCS, NULL };


/* Returns the content of a symbol [variable] */
static char * dlsym_variable (void * handle, char * name)
{
  char ** symbol = dlsym (handle, name);
  return symbol ? * symbol : NULL;
}


/* Returns the address of a symbol [function] */
static rplugin_f * dlsym_function (void * handle, char * name)
{
  return dlsym (handle, name);
}


/* Make a new symbol for a [variable] */
static rplugin_symbol_t * mkvariable (void * handle, char * name)
{
  rplugin_symbol_t * s = NULL;
  void * sym = dlsym_variable (handle, name);
  if (sym)
    {
      s = calloc (1, sizeof (* s));
      s -> name = strdup (name);
      s -> sym  = sym;
    }
  return s;
}


/* Make a new symbol for a [function] */
static rplugin_symbol_t * mkfunction (void * handle, char * name)
{
  rplugin_symbol_t * s = NULL;
  void * sym = dlsym_function (handle, name);
  if (sym)
    {
      s = calloc (1, sizeof (* s));
      s -> name = strdup (name);
      s -> sym  = sym;
    }
  return s;
}


/* Add an item to the table of symbols */
static rplugin_symbol_t ** addsymbol (rplugin_symbol_t * argv [], rplugin_symbol_t * s)
{
  return (rplugin_symbol_t **) vamore ((void **) argv, s);
}


/* Free the table of symbols */
static void freesymbols (rplugin_symbol_t * argv [])
{
  rplugin_symbol_t ** s = argv;
  while (argv && * argv)
    {
      safefree ((* argv) -> name);
      free (* argv ++);
    }
  safefree (s);
}


/* Fill the table of candidate files checking for filenames with "*.so" extension into the directory pointed to by [dir] */
static char ** rls_files (char * dir)
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
	  if (len > RPLUGIN_SO_SUFFIX_LEN && ! strncmp (& file [len - RPLUGIN_SO_SUFFIX_LEN], RPLUGIN_SO_SUFFIX, RPLUGIN_SO_SUFFIX_LEN))
	    argv = argsmore (argv, file);

	  free (file);
	}
      closedir (d);
    }
  return argssort (argv);
}


/* -=-=-=-=-=-=-= API -=-=-=-=-=-=-= */

/* Allocate and initialize a new descriptor to hold info about a plugin */
rplugin_t * rplugin_mk (char * path, int * code, char ** error)
{
  rplugin_t * p = NULL;
  rplugin_symbol_t * s = NULL;
  void * handle;
  char ** names;
  char * symb;

  /* Load the shared object into memory */
  if (! (handle = dlopen (path, RTLD_NOW)))
    {
      if (code)
	* code = RPLUGIN_NOTFOUND;
      if (error)
	* error = strdup (dlerror ());
      return NULL;
    }

  /* Time for memory */
  p = calloc (1, sizeof (rplugin_t));
  p -> path     = strdup (path);
  p -> handle   = handle;
  p -> id       = strdup (basename (path));
  p -> declared = NULL;
  p -> vars     = NULL;
  p -> funs     = NULL;
  p -> state    = RPLUGIN_LOADED;

  p -> id [strlen (p -> id) - RPLUGIN_SO_SUFFIX_LEN] = 0x0;

  /* Load plugin declarations. Stop if not all mandatory plugin declarations have been found */
  names = mandatory;
  symb = (char *) 1;                 /* a true value */
  while (symb && names && * names)
    p -> declared = argsmore (p -> declared, symb = dlsym_variable (handle, * names ++));

  if (! symb)
    {
      if (code)
	* code = RPLUGIN_MISSINGDECL;
      if (error)
	* error = strdup (* -- names);
      rplugin_rm (p);
      return NULL;
    }

  /* Split declarations into arguments and load var/fun symbol tables */
  names = p -> declared;
  while (names && * names)
    {
      char ** defs = argsblanks (* names);  /* split at blanks */
      char ** d = defs;                     /* iterator */

      s = (rplugin_symbol_t *) 1;               /* a true value */
      while (s && d && * d)
	{
	  if (names == p -> declared)
	    p -> vars = addsymbol (p -> vars, s = mkvariable (handle, * d));
	  else
	    p -> funs = addsymbol (p -> funs, s = mkfunction (handle, * d));
	  d ++;
	}

      if (! s)
	{
	  if (code)
	    * code = RPLUGIN_SYMBOLNOTDEFINED;
	  if (error)
	    * error = strdup (* -- d);
	  argsclear (defs);
	  rplugin_rm (p);
	  return NULL;
	}

      argsclear (defs);

      ++ names;
    }

  /* Bless it! */
  gettimeofday (& p -> uptime, NULL);
  p -> state = RPLUGIN_LOADED;

  return p;
}


/* Free allocated memory and resources used to hold a plugin descriptor */
rplugin_t * rplugin_rm (rplugin_t * p)
{
  if (p)
    {
      rplugin_f * halt;
      char * argv [3] = { RPLUGIN_STOP, "now", NULL };

      /* First halt the plugin (if started) */
      if (p -> state == RPLUGIN_STARTED && (halt = rplugin_function (RPLUGIN_STOP, p -> funs)))
	halt (3, argv, NULL);

      safefree (p -> path);
      safefree (p -> id);

      p -> declared = argsclear (p -> declared);
      freesymbols (p -> vars);
      freesymbols (p -> funs);

      if (p -> handle)
	dlclose (p -> handle);

      free (p);
    }

  return NULL;
}


/* Load the plugin passed by filename */
rplugin_t * rplugin_open (char * file)
{
  int err = 0;
  char * error = NULL;
  rplugin_t * p = rplugin_mk (file, & err, & error);
  if (! p)
    safefree (error);
  return p;
}


/* Unload the plugin */
void rplugin_close (rplugin_t * p)
{
  rplugin_rm (p);
}


/* Load the plugins passed by filenames */
rplugin_t ** rplugin_load_files (char * files [])
{
  rplugin_t ** plugins = NULL;
  char ** f = files;
  while (f && * f)
    {
      int err = 0;
      char * error = NULL;
      rplugin_t * p = rplugin_mk (* f ++, & err, & error);
      if (p)
	plugins = rplugin_more (plugins, p);
      else
	safefree (error);
    }
  return plugins;
}


/* Load all the plugins in the given directory */
rplugin_t ** rplugin_load_dir (char * dir, bool verbose)
{
  rplugin_t ** plugins = NULL;
  char ** files = rls_files (dir);
  char ** f = files;
  while (f && * f)
    {
      int err = 0;
      char * error = NULL;
      rplugin_t * p = rplugin_mk (* f, & err, & error);
      if (p)
	plugins = rplugin_more (plugins, p);
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


/* Unload all the plugins in the given directory */
void rplugin_unload (rplugin_t * argv [])
{
  rplugin_t ** p = argv;
  while (p && * p)
    rplugin_close (* p ++);
  safefree (argv);
}


/* Get the list of plugins in the given directory */
char ** rplugin_ls (char * dir)
{
  char ** shobjs = NULL;
  char ** files = rls_files (dir);
  char ** f = files;
  while (f && * f)
    {
      int err = 0;
      char * error = NULL;
      rplugin_t * plugin = rplugin_mk (* f, & err, & error);
      if (plugin)
	{
	  shobjs = argsmore (shobjs, plugin -> path);
	  rplugin_rm (plugin);
	}
      else
	safefree (error);
      f ++;
    }
  argsclear (files);
  return argssort (shobjs);
}


/* Is 'name' a variable? */
char * rplugin_variable (char * name, rplugin_symbol_t * argv [])
{
  while (name && argv && * argv)
    if (! strcmp (name, (* argv) -> name))
      return (* argv) -> sym;
    else
      argv ++;
  return NULL;
}


/* Is 'name' a function? */
rplugin_f * rplugin_function (char * name, rplugin_symbol_t * argv [])
{
  while (name && argv && * argv)
    if (! strcmp (name, (* argv) -> name))
      return (* argv) -> sym;
    else
      argv ++;
  return NULL;
}


bool rplugin_defined (char * name, rplugin_symbol_t * argv [])
{
  return rplugin_variable (name, argv) || rplugin_function (name, argv);
}


rplugin_symbol_t ** rplugin_getvars (rplugin_t * p)
{
  return p && p -> handle && p -> vars ? p -> vars : NULL;
}


/* Check if all the plugin read-only information are accessible */
bool rplugin_hasvars (rplugin_t * p)
{
  return rplugin_getvars (p);
}


/* Get plugin library */
char * rplugin_library (rplugin_t * p)
{
  return rplugin_variable ("library", rplugin_getvars (p));
}


/* Get plugin module */
char * rplugin_module (rplugin_t * p)
{
  return rplugin_variable ("module", rplugin_getvars (p));
}


/* Get plugin author */
char * rplugin_author (rplugin_t * p)
{
  return rplugin_variable ("author", rplugin_getvars (p));
}


/* Fill the table of plugins identifiers (module name) */
char ** rplugin_names (rplugin_t * argv [])
{
  char ** names = NULL;
  while (argv && * argv)
    names = argsuniq (names, rplugin_module (* argv ++));
  return names;
}


/* Fill the table of plugins function names */
char ** rplugin_func_names (rplugin_t * argv [])
{
  char ** names = NULL;
  while (argv && * argv)
    {
      rplugin_symbol_t ** s = (* argv ++) -> funs;
      while (s && * s)
	names = argsuniq (names, (* s ++) -> name);
    }
  return names;
}


/* Lookup for a module name in a table of plugins */
rplugin_t * rplugin_lookup (char * name, rplugin_t * argv [])
{
  while (name && argv && * argv)
    if (! strcmp (rplugin_module (* argv ++), name))
      return * -- argv;
  return NULL;
}


/* Fill the table of plugins function names */
char ** rplugin_functions (rplugin_t * p)
{
  char ** funcs = NULL;
  rplugin_symbol_t ** argv = p ? p -> funs : NULL;
  while (argv && * argv)
    funcs = argsuniq (funcs, (* argv ++) -> name);
  return funcs;
}


/* Check is plugin implement a given function */
bool rplugin_implement (rplugin_t * p, char * name)
{
  rplugin_symbol_t ** argv = p ? p -> funs : NULL;
  while (name && argv && * argv)
    if (! strcmp ((* argv ++) -> name, name))
      return true;
  return false;
}


/* Check at least one plugin has a given function defined */
bool rplugin_has (rplugin_t * argv [], char * name)
{
  while (name && argv && * argv)
    if (rplugin_implement (* argv ++, name))
      return true;
  return false;
}


/* Counting function for better rendering */

/* Max id name length */
unsigned rplugin_maxi (rplugin_t * argv [])
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
unsigned rplugin_maxl (rplugin_t * argv [])
{
  unsigned n = 0;
  char * s;
  while (argv && * argv)
    if ((s = rplugin_library (* argv ++)))
      n = RMAX (n, strlen (s));
  return n;
}


/* Max module name length */
unsigned rplugin_maxm (rplugin_t * argv [])
{
  unsigned n = 0;
  char * s;
  while (argv && * argv)
    if ((s = rplugin_module (* argv ++)))
      n = RMAX (n, strlen (s));
  return n;
}


/* Max author name length */
unsigned rplugin_maxa (rplugin_t * argv [])
{
  unsigned n = 0;
  char * s;
  while (argv && * argv)
    if ((s = rplugin_author (* argv ++)))
      n = RMAX (n, strlen (s));
  return n;
}


/* Rendering functions */

/* General information */
void rplugin_print_info (rplugin_t * p, unsigned seq, unsigned maxi, unsigned maxl, unsigned maxm, unsigned maxa)
{
  if (rplugin_hasvars (p))
    printf ("%3d %-*.*s %-*.*s %-*.*s %-*.*s (%s)\n",
	    seq,
	    maxi, maxi, p -> id,
	    maxl, maxl, rplugin_library (p),
	    maxm, maxm, rplugin_module (p),
	    maxa, maxa, rplugin_author (p),
	    basename (p -> path));
}


/* Unique identifier in terms of Library / Module / Author */
void rplugin_print_id (rplugin_t * p, unsigned seq, unsigned maxl, unsigned maxm, unsigned maxa)
{
  if (rplugin_hasvars (p))
    printf ("%3d %-*.*s %-*.*s %-*.*s (%s)\n", seq,
	    maxl, maxl, rplugin_library (p),
	    maxm, maxm, rplugin_module (p),
	    maxa, maxa, rplugin_author (p),
	    basename (p -> path));
}


/* General information */
void rplugin_print_infos (rplugin_t * argv [])
{
  unsigned maxi = rplugin_maxi (argv);
  unsigned maxl = rplugin_maxl (argv);
  unsigned maxm = rplugin_maxm (argv);
  unsigned maxa = rplugin_maxa (argv);
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
    rplugin_print_info (* argv ++, ++ seq, maxi, maxl, maxm, maxa);
}


void rplugin_print_ids (rplugin_t * argv [])
{
  unsigned maxl = rplugin_maxl (argv);
  unsigned maxm = rplugin_maxm (argv);
  unsigned maxa = rplugin_maxa (argv);
  unsigned seq = 0;

  /* Table Header */
  if (argv)
    printf (" ## %-*.*s %-*.*s %-*.*s %s\n", maxl, maxl, "Library", maxm, maxm, "Module", maxa, maxa, "Author", "Filename");

  while (argv && * argv)
    rplugin_print_id (* argv ++, ++ seq, maxl, maxm, maxa);
}
