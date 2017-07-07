/* System headers */
#include <stdio.h>
#include <libgen.h>
#include <sys/utsname.h>

/* Project headers */
#include "roptions.h"
#include "plugins.h"
#include "args.h"
#include "varrays.h"
#include "rctype.h"
#include "rtest.h"


/* Program version */
#define _NAME_      "plugins manager"
#define _VERSION_   "0.1.0"

/* Default option (if none was specified) */
#define OPT_DEFAULT OPT_SHOW_ID
#undef DEFAULT_DIR
#define DEFAULT_DIR "../../plugins"


/* All the command line long options are here */
typedef enum
{
  /* Miscellanea */
  OPT_HELP         = 'h',
  OPT_VERSION      = 'v',
  OPT_VERBOSE      = 'V',
  OPT_QUIET        = 'q',

  /* Plugins */

  /* Directory */
  OPT_GET_DIR      = 'g',
  OPT_SET_DIR      = 'd',

  /* List */
  OPT_LIST_PLUGINS = 'l',    /* List                       */

  /* Load/Unload */
  OPT_LOAD_UNLOAD  = 'L',    /* Load/Unload all            */

  /* Show variables */
  OPT_SHOW_GENERAL = 'G',    /* General information        */
  OPT_SHOW_ID      = 'I',    /* Id (library/module)        */

  /* Finger */
  OPT_ADD_PLUGIN   = 'p',    /* Include item               */
  OPT_DEL_PLUGIN   = 'P',    /* Exclude item               */

  /* Functions */

  /* Finger */
  OPT_ADD_FUNCTION = 'f',    /* Include item               */
  OPT_DEL_FUNCTION = 'F',    /* Exclude item               */

  /* Execute */
  OPT_EXECUTE      = 'x',    /* Execute functions          */

  /* Item counters */
  OPT_ITEMS        = 'n',    /* initial # of items */
  OPT_ITEMS_0      = '0',    /* 10 ^ 0 */
  OPT_ITEMS_1      = '1',    /* 10 ^ 1 */
  OPT_ITEMS_2      = '2',    /* 10 ^ 2 */
  OPT_ITEMS_3      = '3',    /* 10 ^ 3 */
  OPT_ITEMS_4      = '4',    /* 10 ^ 4 */
  OPT_ITEMS_5      = '5',    /* 10 ^ 5 */
  OPT_ITEMS_6      = '6',    /* 10 ^ 6 */
  OPT_ITEMS_7      = '7',    /* 10 ^ 7 */
  OPT_ITEMS_8      = '8',    /* 10 ^ 8 */
  OPT_ITEMS_9      = '9',    /* 10 ^ 9 */

  /* Run counters */
  OPT_RUNS         = 'r',

} ropt_t;


/* GNU Long Options */
static struct option lopts [] =
{
  /* Miscellanea */
  { "help",         no_argument,       NULL, OPT_HELP         },
  { "version",      no_argument,       NULL, OPT_VERSION      },
  { "verbose",      no_argument,       NULL, OPT_VERBOSE      },
  { "quiet",        no_argument,       NULL, OPT_QUIET        },

  /* Plugins */

  /* Directory */
  { "get-dir",      no_argument,       NULL, OPT_GET_DIR      },
  { "set-dir",      required_argument, NULL, OPT_SET_DIR      },

  /* List */
  { "plugins",      no_argument,       NULL, OPT_LIST_PLUGINS },

  /* Load/Unload all */
  { "load-unload",  no_argument,       NULL, OPT_LOAD_UNLOAD  },

  /* Show variables */
  { "show-general", no_argument,       NULL, OPT_SHOW_GENERAL },
  { "show-id",      no_argument,       NULL, OPT_SHOW_ID      },

  /* Finger */
  { "add-plugin",   required_argument, NULL, OPT_ADD_PLUGIN   },
  { "del-plugin",   required_argument, NULL, OPT_DEL_PLUGIN   },

  /* Functions */

  /* Finger */
  { "add-function", required_argument, NULL, OPT_ADD_FUNCTION },
  { "del-function", required_argument, NULL, OPT_DEL_FUNCTION },

  /* Execute */
  { "execute",      no_argument,       NULL, OPT_EXECUTE      },

  /* Item counters */
  { "items",        required_argument, NULL, OPT_ITEMS        },
  { "one",          no_argument,       NULL, OPT_ITEMS_0      },
  { "ten",          no_argument,       NULL, OPT_ITEMS_1      },
  { "hundred",      no_argument,       NULL, OPT_ITEMS_2      },
  { "thousand",     no_argument,       NULL, OPT_ITEMS_3      },
  { "10-thousand",  no_argument,       NULL, OPT_ITEMS_4      },
  { "100-thousand", no_argument,       NULL, OPT_ITEMS_5      },
  { "million",      no_argument,       NULL, OPT_ITEMS_6      },
  { "10-million",   no_argument,       NULL, OPT_ITEMS_7      },
  { "100-million",  no_argument,       NULL, OPT_ITEMS_8      },
  { "billion",      no_argument,       NULL, OPT_ITEMS_9      },

  /* Run counters */
  { "runs",         required_argument, NULL, OPT_RUNS         },

  /* End of options */
  { NULL,           0,                 NULL, 0                }
};


/* Print plugins names */
static void print_names (char * progname, unsigned choice, char * dir, char * subset [])
{
  plugin_t ** loaded;

  if (subset)
    loaded = rload_files (subset);
  else
    loaded = rload_plugins (dir, false);

  if (loaded)
    {
      switch (choice)
	{
	case OPT_SHOW_GENERAL: rp_print_infos (loaded); break;
	case OPT_SHOW_ID:      rp_print_ids (loaded);   break;
	default:                                        break;
	}

      runload_plugins (loaded);
    }
  else
    printf ("%s: no plugin found in directory %s\n", progname, dir);
}


#if defined(ROCCO)
static void print_label (char * plugin, char * function)
{
  char running [256];
  unsigned r;

  sprintf (running, "%s/%s", plugin, function);
  printf ("%s ", running);
  r = 19 - strlen (running);

  while (r --)
    printf (".");
  printf (" ");
}


static char * sprintn (unsigned n, unsigned digits)
{
  static char numbers [20] [10];
  static unsigned i = 0;

  char * s = numbers [i ++ % 20];
  char fmt [10];

  sprintf (fmt, "%%-%uu", digits);
  sprintf (s, fmt, n);
  return s;
}
#endif /* ROCCO */


/* Build the list of plugins to include/exclude */
static char ** choose (char * progname, char * files [], char * included [], char * excluded [])
{
  char ** subset = NULL;

  /* Included first */
  if (included)
    {
      char ** a = included;
      {
	while (a && * a)
	  {
	    if (isnumeric (* a))
	      {
		unsigned n = atoi (* a);
		if (n && n < arrlen (files) + 1)
		  subset = argsuniq (subset, files [n - 1]);
		else
		  {
		    printf ("%s: [%u] is out of range (valid are in range [1-%u])\n", progname, n, arrlen (files));
		    arrclear (subset, free);
		    return NULL;
		  }
	      }
	    else
	      {
		char ** f = files;
		bool found = false;
		while (f && * f)
		  {
		    char * b = basename (* f);
		    if (! strncmp (* a, b, strlen (* a)) && strlen (b) == strlen (* a) + SO_SUFFIX_LEN)
		      {
			subset = argsuniq (subset, * f);
			found = true;
			break;
		      }
		    f ++;
		  }

		if (! found)
		  {
		    printf ("%s: [%s] is not a known plugin name\n", progname, * a);
		    arrclear (subset, free);
		    return NULL;
		  }
	      }
	    a ++;
	  }
      }
    }
  else
    {
      printf ("Not yet implemented\n");
    }

  return subset;
}


/* Call the functions of plugins */
static void rp_call (plugin_t * loaded [], char * subset [])
{
  plugin_t ** p = loaded;
  while (p && * p)
    {
      symbol_t ** s = (* p) -> func;
      while (s && * s)
	{
	  if (! subset || argsexists (subset, (* s) -> name))
	    {
	      call_t * fun = function ((* s) -> name, (* p) -> func);
	      char * argv [3] = { (* s) -> name, NULL, NULL };
	      int argc = 1;

	      printf (" calling %s/%s ... ", (* p) -> id, (* s) -> name);

	      if (! fun (argc, argv, NULL))
		printf (" Ok\n");
	      else
		printf (" No\n");
	    }
	  s ++;
	}
      p ++;
    }
}


/* Attempt to do what has been required by the user */
static void doit (char * progname, unsigned choice, char * dir, char * files [], char * calls [], bool verbose, bool quiet)
{
  plugin_t ** loaded = NULL;
  struct utsname u;

  switch (choice)
    {
    case OPT_GET_DIR:
    case OPT_SET_DIR:
      if (! quiet)
	printf ("%s: current plugins directory %s\n", progname, dir);
      break;

    case OPT_LIST_PLUGINS:
      if (files)
	argsrows (files);
      else
	printf ("%s: no plugins in directory %s\n", progname, dir);
      break;

    case OPT_LOAD_UNLOAD:
      printf ("Trying to load all plugins in %s ... ", dir);
      loaded = rload_plugins (dir, verbose);
      if (loaded)
	{
	  printf ("Ok! %u loaded\n", arrlen (loaded));
	  printf ("Unload all plugins ... ");
	  printf ("done!\n");
	}
      else
	printf ("%s: no plugins in directory %s\n", progname, dir);
      break;

      /* Show Variables/Functions */
    case OPT_SHOW_GENERAL:
    case OPT_SHOW_ID:
      print_names (progname, choice, dir, files);
      break;

    case OPT_EXECUTE:

      /* Welcome on board! */
      uname (& u);
      printf ("%s %s %s %s %s\n", u . sysname, u . nodename, u . release, u . version, u . machine);
      printf ("\n");

      loaded = rload_files (files);
      rp_call (loaded, calls);
      break;

    default:
      break;
    }

  /* Memory cleanup */
  runload_plugins (loaded);
}


/* Display version information */
static void _version_ (char * progname, char * version)
{
  printf ("%s version %s built on %s %s\n", progname, version, __DATE__, __TIME__);
  fflush (stdout);
}


/* Display the syntax for using this program */
static void _usage_ (char * progname, char * version, struct option * options)
{
  /* Get the longest option name for optimal rendering */
  unsigned n = optmax (options);

  printf ("%s %s, %s\n", progname, version, _NAME_);
  printf ("\n");

  printf ("Usage: %s [options]\n", progname);
  printf ("\n");

  printf ("  Miscellanea:\n");
  usage_item (options, n, OPT_HELP,         "show this message and exit");
  usage_item (options, n, OPT_VERSION,      "show version information and exit");
  usage_item (options, n, OPT_VERBOSE,      "run tests verbosely");
  usage_item (options, n, OPT_QUIET,        "run tests quietly");
  printf ("\n");

  printf ("  Plugins:\n");

  printf ("    Directory:\n");
  usage_item (options, n, OPT_GET_DIR,      "show current plugins directory");
  usage_item (options, n, OPT_SET_DIR,      "set plugins directory");
  printf ("\n");

  printf ("    List:\n");
  usage_item (options, n, OPT_LIST_PLUGINS, "list plugins");
  printf ("\n");

  printf ("    Load/Unload:\n");
  usage_item (options, n, OPT_LOAD_UNLOAD,  "load and unload all plugins (useful to check for memory leaks)");
  printf ("\n");

  printf ("    Show Variables:\n");
  usage_item (options, n, OPT_SHOW_GENERAL, "all general information (default)");
  usage_item (options, n, OPT_SHOW_ID,      "id (library/module names)");
  printf ("\n");

  printf ("    Finger:\n");
  usage_item (options, n, OPT_ADD_PLUGIN,   "include plugin (repeatable)");
  usage_item (options, n, OPT_DEL_PLUGIN,   "exclude plugin (repeatable)");
  printf ("\n");

  printf ("    Functions:\n");
  usage_item (options, n, OPT_ADD_FUNCTION, "include function (repeatable)");
  usage_item (options, n, OPT_DEL_FUNCTION, "exclude function (repeatable)");
  printf ("\n");

  printf ("    Execute:\n");
  usage_item (options, n, OPT_EXECUTE,      "execute functions");
  printf ("\n");

  printf ("  Unit Tests:\n");
  printf ("\n");

  printf ("  Item counters: (default %.0f)\n", INITIALS);
  usage_item (options, n, OPT_ITEMS,        "set the initial number of items per test");
  usage_item (options, n, OPT_ITEMS_0,      "one item                   (1e0)");
  usage_item (options, n, OPT_ITEMS_1,      "ten items                  (1e1)");
  usage_item (options, n, OPT_ITEMS_2,      "one hundred items          (1e2)");
  usage_item (options, n, OPT_ITEMS_3,      "one thousand items         (1e3)");
  usage_item (options, n, OPT_ITEMS_4,      "ten thousand items         (1e4)");
  usage_item (options, n, OPT_ITEMS_5,      "one hundred thousand items (1e5)");
  usage_item (options, n, OPT_ITEMS_6,      "one million items          (1e6)");
  usage_item (options, n, OPT_ITEMS_7,      "ten million items          (1e7)");
  usage_item (options, n, OPT_ITEMS_8,      "one hundred million items  (1e8)");
  usage_item (options, n, OPT_ITEMS_9,      "one billion items          (1e9)");
  printf ("\n");

  printf ("  Run counters: (default %u)\n", RUNS);
  usage_item (options, n, OPT_RUNS,    "Set the number of runs per test");
}


/* Display/Select/Execute Plugins and Unit Tests */
int main (int argc, char * argv [])
{
  char * progname  = basename (argv [0]);    /* notice program name */
  char * sopts     = optlegitimate (lopts);  /* short option list   */

  /* Booleans */
  bool verbose     = false;
  bool quiet       = false;

  /* Plugins */
  char * dir       = DEFAULT_DIR;            /* directory */
  char ** files    = NULL;                   /* filenames */
  char ** included = NULL;
  char ** excluded = NULL;

  /* Functions */
  char ** calls    = NULL;

  /* Items counters */
  unsigned items   = INITIALS;               /* initial # of items per test */

  /* Run counters */
  unsigned runs    = RUNS;                   /* # of run per test           */

  unsigned choice  = OPT_DEFAULT;
  int option;

  /* Set unbuffered stdout */
  setvbuf (stdout, NULL, _IONBF, 0);

  /* Parse command-line options */
  argv [0] = progname;
  while ((option = getopt_long (argc, argv, sopts, lopts, NULL)) != -1)
    {
      switch (option)
	{
	default:
	  printf ("Try '%s --help' for more information.\n", progname);
	  return 1;

	  /* Miscellanea */
	case OPT_HELP:         _usage_ (progname, _VERSION_, lopts);   return 0;
	case OPT_VERSION:      _version_ (progname, _VERSION_);        return 0;
	case OPT_VERBOSE:      verbose = true;                         break;
	case OPT_QUIET:        quiet   = true;                         break;

	  /* Plugins section */

	  /* Directory */
        case OPT_GET_DIR:      choice = option;                        break;
        case OPT_SET_DIR:      dir = optarg; choice = option;          break;

	  /* List */
        case OPT_LIST_PLUGINS: choice = option;                        break;

	  /* Load/Unload all */
        case OPT_LOAD_UNLOAD:  choice = option;                        break;

	  /* Show Variables/Functions */
        case OPT_SHOW_GENERAL:
        case OPT_SHOW_ID:      choice = option;                        break;

	  /* Finger */
        case OPT_ADD_PLUGIN:   included = argsuniq (included, optarg); break;
        case OPT_DEL_PLUGIN:   excluded = argsuniq (excluded, optarg); break;

	  /* Functions */
        case OPT_ADD_FUNCTION: calls = argsuniq (calls, optarg); break;
#if defined(ROCCO)
        case OPT_DEL_FUNCTION: xxx = argsuniq (xxx, optarg); break;
#endif /* ROCCO */

	  /* Execute */
        case OPT_EXECUTE:      choice = option;                        break;

	  /* Item counters */
	case OPT_ITEMS:   items = atoi (optarg); break;
	case OPT_ITEMS_0: items = 1e0;           break;
	case OPT_ITEMS_1: items = 1e1;           break;
	case OPT_ITEMS_2: items = 1e2;           break;
	case OPT_ITEMS_3: items = 1e3;           break;
	case OPT_ITEMS_4: items = 1e4;           break;
	case OPT_ITEMS_5: items = 1e5;           break;
	case OPT_ITEMS_6: items = 1e6;           break;
	case OPT_ITEMS_7: items = 1e7;           break;
	case OPT_ITEMS_8: items = 1e8;           break;
	case OPT_ITEMS_9: items = 1e9;           break;

	  /* Run counters */
	case OPT_RUNS:    runs  = atoi (optarg); break;
	}
    }

  /* Check for unsupported arguments */
  if (argc != optind)
    {
      printf ("Unsupported arguments: %u\n", argc - optind);
      while (argc != optind)
	printf ("%s\n", argv [optind ++]);
      return 1;
    }

  /* Avoid to run with 0 initial items */
  if (! items)
    items = INITIALS;

  if (! runs)
    runs = RUNS;

  /* Evaluate the list of available plugins */
  files = rls_plugins (dir);
  if (files)
    {
      /* Attempt to do what has been required by the user */
      if (included || excluded)
	{
	  char ** subset = choose (progname, files, included, excluded);
	  if (subset)
	    {
	      arrclear (files, free);
	      files = subset;

	      /* Go! */
	      doit (progname, choice, dir, files, calls, verbose, quiet);
	    }
	  else
	    printf ("%s: Empty subset\n", progname);
	}
      else
	/* Go! */
	doit (progname, choice, dir, files, calls, verbose, quiet);
    }
  else
    printf ("%s: no plugins in directory %s\n", progname, dir);

  /* Memory cleanup */
  arrclear (calls, free);
  arrclear (files, free);
  arrclear (excluded, free);
  arrclear (included, free);

  return 0;
}
