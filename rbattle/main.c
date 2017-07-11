/* System headers */
#include <stdio.h>
#include <libgen.h>
#include <sys/utsname.h>

/* Project headers */
#include "roptions.h"
#include "args.h"
#include "rctype.h"
#include "plugins.h"
#include "rtest.h"
#include "rbattle.h"


/* Program name/version */
#define _NAME_      "At the end there is only one"
#define _VERSION_   "0.1.0"

#define TOOSLOW   10    /* too slow condition limit            */
#define REPEAT    100   /* # of items to add for test repetion */
#define NMORE     500   /* # of items to add at the next run   */

/* Default option (if none was specified) */
#define OPT_DEFAULT OPT_EXECUTE


/* All the command line long options are here */
typedef enum
{
  /* Miscellanea */
  OPT_HELP         = 'h',
  OPT_VERSION      = 'v',
  OPT_VERBOSE      = 'V',
  OPT_QUIET        = 'q',

  /* Test Suite */

  /* List */
  OPT_LIST_TESTS   = 's',    /* List Test Suite     */

  /* Finger */
  OPT_ADD_TEST     = 't',    /* Include Test        */
  OPT_DEL_TEST     = 'T',    /* Exclude Test        */

  /* Execute */
  OPT_EXECUTE      = 'x',    /* Execute Test Suite  */

  /* Plugins */

  /* Directory */
  OPT_GET_DIR      = 'g',
  OPT_SET_DIR      = 'd',

  /* List */
  OPT_LIST_PLUGINS = 'L',    /* List                */

  /* Load/Unload */
  OPT_LOAD_UNLOAD  = 'k',    /* Check Load/Unload   */

  /* Show variables */
  OPT_SHOW_GENERAL = 'G',    /* General information */
  OPT_SHOW_ID      = 'I',    /* Id (library/module) */

  /* Finger */
  OPT_ADD_PLUGIN   = 'p',    /* Include plugin      */
  OPT_DEL_PLUGIN   = 'P',    /* Exclude plugin      */

  /* Item counters */
  OPT_ITEMS        = 'n',    /* initial # of items  */
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
  OPT_SLOW         = 'S',
  OPT_REPEAT       = 'R',
  OPT_MORE         = 'M',

  /* Display */
  OPT_LESS         = 'l',
  OPT_SHOW         = 'b',

} ropt_t;


/* GNU Long Options */
static struct option lopts [] =
{
  /* Miscellanea */
  { "help",         no_argument,       NULL, OPT_HELP         },
  { "version",      no_argument,       NULL, OPT_VERSION      },
  { "verbose",      no_argument,       NULL, OPT_VERBOSE      },
  { "quiet",        no_argument,       NULL, OPT_QUIET        },

  /* Test Suite */

  /* List */
  { "suite",        no_argument,       NULL, OPT_LIST_TESTS   },

  /* Finger */
  { "add-test",     required_argument, NULL, OPT_ADD_TEST     },
  { "del-test",     required_argument, NULL, OPT_DEL_TEST     },

  /* Execute */
  { "execute",      no_argument,       NULL, OPT_EXECUTE      },

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

  { "tooslow",      required_argument, NULL, OPT_SLOW         },
  { "repeat",       required_argument, NULL, OPT_REPEAT       },
  { "more",         required_argument, NULL, OPT_MORE         },

  /* Booleans */
  { "less",         no_argument,       NULL, OPT_LESS         },
  { "show",         no_argument,       NULL, OPT_SHOW         },

  /* End of options */
  { NULL,           0,                 NULL, 0                }
};


/* Print plugins names */
static void print_names (char * progname, unsigned choice, char * dir, char * subset [])
{
  rplugin_t ** loaded = subset ? rplugin_load_files (subset) : rplugin_load_dir (dir, false);
  if (loaded)
    {
      switch (choice)
	{
	case OPT_SHOW_GENERAL: rplugin_print_infos (loaded); break;
	case OPT_SHOW_ID:      rplugin_print_ids (loaded);   break;
	default:                                             break;
	}

      rplugin_unload (loaded);
    }
  else
    printf ("%s: no plugin found in directory %s\n", progname, dir);
}


/* Looukup for an unique plugin name */
static int find_by_name (char * name, char * rargv [])
{
  unsigned n = arrlen (rargv);
  unsigned i;
  if (name)
    for (i = 0; i < n; i ++)
      if (! strncmp (name, basename (rargv [i]), RMAX (strlen (name), strlen (basename (rargv [i])) - RPLUGIN_SO_SUFFIX_LEN)))
	return i;
  return -1;
}


/*
 * A valid plugin id is:
 *  * if numeric in the range [1-n] where n is the number of found plugins
 *  * match a filename
 */
static int rp_valid_id (char * id, char * rargv [])
{
  return isnumeric (id) && atoi (id) && atoi (id) <= arrlen (rargv) ? atoi (id) - 1 : find_by_name (id, rargv);
}


/* Attempt to do what has been required by the user */
static void doit (char * progname, unsigned choice,
		  char * dir, char * files [],
		  char * suite [], unsigned items,
		  unsigned runs, unsigned nslow, unsigned repeat, unsigned nmore,
		  bool verbose, bool quiet, bool less, bool show)
{
  rplugin_t ** loaded = NULL;
  struct utsname u;

  switch (choice)
    {
    case OPT_EXECUTE:

      /* Welcome on board! */
      uname (& u);
      printf ("%s %s %s %s %s\n", u . sysname, u . nodename, u . release, u . version, u . machine);
      printf ("\n");

      /* Initialize/Run/Terminate all the implementations under test */
#if defined(ROCCO)
      sw_done (run_suite (suite, sw_init (files, items, verbose), items, runs, verbose, quiet), verbose);
#else
      sw_t ** running;

      /* Initialize all the implementations under test */
      running = sw_init (files, items, verbose);

      /* Run all tests for all the implementations to benchmark */
      run_all_tests (running, items, runs, nslow, repeat, nmore, verbose, quiet, less, show);

      /* Terminate all the implementations under test */
      sw_done (running, verbose);

#endif /* ROCCO */
      break;

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
      if (verbose)
	printf ("\n");
      loaded = rplugin_load_dir (dir, verbose);
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

    default:
      break;
    }

  /* Memory cleanup */
  rplugin_unload (loaded);
}


/*
 * Build the suite of tests to run.
 */
static char ** build_suite (char * progname, char * included [], char * excluded [])
{
  char ** names  = included ? included : excluded;       /* User-included items have priority over user-excluded */
  char ** subset = included ? NULL : rsuite_names ();    /* Nothing or everything but these */

  /* Loop over plugin names to define the subset of user selected */
  while (names && * names)
    {
      rtest_t * t = rsuite_valid (* names);
      if (t)
	subset = included ? argsuniq (subset, t -> name) : argsless (subset, t -> name);
      else
	{
	  printf ("%s: [%s] is not a valid id\n", progname, * names);
	  argsclear (subset);
	  return NULL;
	}
      names ++;
    }

  return subset;
}


/*
 * Build the list of plugins to include/exclude.
 *
 * The items included by the user have higher priority and they assume the meaning of "nothing but these".
 * The items excluded by the user have lower priority and they assume the meaning of "everything but these".
 *
 */
static char ** choose (char * progname, char * files [], char * included [], char * excluded [])
{
  char ** names  = included ? included : excluded;       /* User-included items have priority over user-excluded */
  char ** subset = included ? NULL : argsdup (files);    /* Nothing or everything but these */

  /* Loop over plugin names to define the subset of user selected */
  while (names && * names)
    {
      int i = rp_valid_id (* names, files);
      if (i != -1)
	subset = included ? argsuniq (subset, files [i]) : argsless (subset, files [i]);
      else
	{
	  printf ("%s: [%s] is not a valid id\n", progname, * names);
	  argsclear (subset);
	  return NULL;
	}
      names ++;
    }

  return subset;
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

  printf ("  Test Suite:\n");
  usage_item (options, n, OPT_LIST_TESTS,   "list test suite");
  usage_item (options, n, OPT_ADD_TEST,     "include test (repeatable)");
  usage_item (options, n, OPT_DEL_TEST,     "exclude test (repeatable)");
  usage_item (options, n, OPT_EXECUTE,      "execute test suite (default)");
  printf ("\n");

  printf ("  Plugins:\n");
  usage_item (options, n, OPT_LOAD_UNLOAD,  "load and unload all plugins (useful to check for memory leaks)");
  usage_item (options, n, OPT_GET_DIR,      "show current plugins directory");
  usage_item (options, n, OPT_SET_DIR,      "set plugins directory");
  usage_item (options, n, OPT_LIST_PLUGINS, "list plugins");
  usage_item (options, n, OPT_SHOW_GENERAL, "all general information");
  usage_item (options, n, OPT_SHOW_ID,      "id (library/module names)");
  usage_item (options, n, OPT_ADD_PLUGIN,   "include plugin (repeatable)");
  usage_item (options, n, OPT_DEL_PLUGIN,   "exclude plugin (repeatable)");
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
  usage_item (options, n, OPT_SLOW,    "Set the occurrences to mark an implementation too slow (default 10)");
  printf ("\n");

  usage_item (options, n, OPT_LESS,    "Enable less mode (do not show each test execution)");
  usage_item (options, n, OPT_SHOW,    "Enable verbose mode to show each test execution");
}


/* Run benchmarks */
int main (int argc, char * argv [])
{
  char * progname  = basename (argv [0]);    /* notice program name */
  char * sopts     = optlegitimate (lopts);  /* short option list   */

  /* Booleans */
  bool verbose     = false;
  bool less        = false;
  bool quiet       = false;
  bool show        = true;

  /* Test Suite */
  char ** enabled  = NULL;
  char ** disabled = NULL;

  /* Plugins */
  char * dir       = RPLUGIN_DEFAULT_DIR;    /* plugins directory */
  char ** files    = NULL;                   /* available plugins */
  char ** included = NULL;
  char ** excluded = NULL;

  /* Items counters */
  unsigned items   = INITIALS;               /* initial # of items per test */

  /* Run counters */
  unsigned runs    = RUNS;                   /* # of run per test           */
  unsigned nslow   = TOOSLOW;                /* too slow limit per test          */
  unsigned repeat  = REPEAT;                 /* add # items (repetition)         */
  unsigned nmore   = NMORE;                  /* # of items to increment per test */

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
	  printf ("Try '%s --help' for more information.\n", progname); return 1;

	  /* Miscellanea */
	case OPT_HELP:         _usage_ (progname, _VERSION_, lopts);    return 0;
	case OPT_VERSION:      _version_ (progname, _VERSION_);         return 0;
	case OPT_VERBOSE:      verbose = true;                          break;
	case OPT_QUIET:        quiet   = true;                          break;

	  /* Test Suite */

	  /* List */
        case OPT_LIST_TESTS:   rsuite_print_all ();                     return 0;

	  /* Finger */
        case OPT_ADD_TEST:     enabled  = argsuniq (enabled, optarg);   break;
        case OPT_DEL_TEST:     disabled = argsuniq (disabled, optarg);  break;

	  /* Execute */
        case OPT_EXECUTE:      choice = option;                         break;

	  /* Plugins */

	  /* Directory */
        case OPT_GET_DIR:      choice = option;                         break;
        case OPT_SET_DIR:      dir = optarg; choice = option;           break;

	  /* List */
        case OPT_LIST_PLUGINS: choice = option;                         break;

	  /* Load/Unload all */
        case OPT_LOAD_UNLOAD:  choice = option;                         break;

	  /* Show Variables/Functions */
        case OPT_SHOW_GENERAL:
        case OPT_SHOW_ID:      choice = option;                         break;

	  /* Finger */
        case OPT_ADD_PLUGIN:   included = argsuniq (included, optarg);  break;
        case OPT_DEL_PLUGIN:   excluded = argsuniq (excluded, optarg);  break;

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
	case OPT_RUNS:   runs   = atoi (optarg); break;
	case OPT_SLOW:   nslow  = atoi (optarg); break;
	case OPT_REPEAT: repeat = atoi (optarg); break;
	case OPT_MORE:   nmore  = atoi (optarg); break;

	  /* Display */
	case OPT_LESS: less  = ! less;             break;
	case OPT_SHOW: show  = ! show;             break;
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

  if (! nslow)
    nslow = TOOSLOW;

  files = rplugin_ls (dir);      /* plugins available in dir */
  if (files)
    {
      /* Build the suite to run */
      char ** suite = build_suite (progname, enabled, disabled);
      if (suite)
	{
	  /* Build the subset of plugins and go! */
	  char ** subset = choose (progname, files, included, excluded);
	  if (subset)
	    doit (progname, choice, dir, subset, suite, items, runs, nslow, repeat, nmore,
		  verbose, quiet, less, show);
	  else
	    printf ("%s: Empty subset\n", progname);
	  argsclear (subset);
	}
      else
	printf ("%s: no test to run\n", progname);
      argsclear (suite);
    }
  else
    printf ("%s: no plugin found in directory %s\n", progname, dir);

  /* Memory cleanup */
  argsclear (disabled);
  argsclear (enabled);
  argsclear (excluded);
  argsclear (included);
  argsclear (files);


  return 0;
}
