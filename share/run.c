/* System headers */
#include <stdio.h>
#include <libgen.h>
#include <getopt.h>

/* Project headers */
#include "roptions.h"
#include "varrays.h"
#include "args.h"
#include "rtest.h"


/* Program version */
#define _NAME_      "Run Unit Tests and Test Suite over this implementation"
#define _VERSION_   "0.1.0"

/* Default option (if none was specified) */
#define OPT_DEFAULT OPT_LIST_UNIT


/* All the command line long options are here */
typedef enum
{
  /* Miscellanea */
  OPT_HELP       = 'h',
  OPT_VERSION    = 'v',
  OPT_VERBOSE    = 'V',
  OPT_QUIET      = 'q',

  /* Operations */
  OPT_LIST_UNIT  = 'u',
  OPT_LIST_SUITE = 's',
  OPT_RUN_UNIT   = 'x',
  OPT_RUN_SUITE  = 'X',

  /* Finger */
  OPT_INCLUDE    = 'i',    /* include item */
  OPT_EXCLUDE    = 'e',    /* exclude item */

  /* Item counters */
  OPT_ITEMS      = 'n',    /* initial # of items */
  OPT_ITEMS_0    = '0',    /* 10 ^ 0 */
  OPT_ITEMS_1    = '1',    /* 10 ^ 1 */
  OPT_ITEMS_2    = '2',    /* 10 ^ 2 */
  OPT_ITEMS_3    = '3',    /* 10 ^ 3 */
  OPT_ITEMS_4    = '4',    /* 10 ^ 4 */
  OPT_ITEMS_5    = '5',    /* 10 ^ 5 */
  OPT_ITEMS_6    = '6',    /* 10 ^ 6 */
  OPT_ITEMS_7    = '7',    /* 10 ^ 7 */
  OPT_ITEMS_8    = '8',    /* 10 ^ 8 */
  OPT_ITEMS_9    = '9',    /* 10 ^ 9 */

  /* Run counters */
  OPT_RUNS       = 'r',

} ropt_t;


/* GNU Long Options */
static struct option lopts [] =
{
  /* Miscellanea */
  { "help",         no_argument,       NULL, OPT_HELP       },
  { "version",      no_argument,       NULL, OPT_VERSION    },
  { "verbose",      no_argument,       NULL, OPT_VERBOSE    },
  { "quiet",        no_argument,       NULL, OPT_QUIET      },

  /* Operations */
  { "unit-list",    no_argument,       NULL, OPT_LIST_UNIT  },
  { "suite-list",   no_argument,       NULL, OPT_LIST_SUITE },
  { "run-unit",     no_argument,       NULL, OPT_RUN_UNIT   },
  { "run-suite",    no_argument,       NULL, OPT_RUN_SUITE  },

  /* Finger */
  { "include",      required_argument, NULL, OPT_INCLUDE    },
  { "exclude",      required_argument, NULL, OPT_EXCLUDE    },

  /* Item counters */
  { "items",        required_argument, NULL, OPT_ITEMS      },
  { "one",          no_argument,       NULL, OPT_ITEMS_0    },
  { "ten",          no_argument,       NULL, OPT_ITEMS_1    },
  { "hundred",      no_argument,       NULL, OPT_ITEMS_2    },
  { "thousand",     no_argument,       NULL, OPT_ITEMS_3    },
  { "10-thousand",  no_argument,       NULL, OPT_ITEMS_4    },
  { "100-thousand", no_argument,       NULL, OPT_ITEMS_5    },
  { "million",      no_argument,       NULL, OPT_ITEMS_6    },
  { "10-million",   no_argument,       NULL, OPT_ITEMS_7    },
  { "100-million",  no_argument,       NULL, OPT_ITEMS_8    },
  { "billion",      no_argument,       NULL, OPT_ITEMS_9    },

  /* Run counters */
  { "runs",         required_argument, NULL, OPT_RUNS       },

  /* End of options */
  { NULL,           0,                 NULL, 0              }
};


/* Finger at Unit Tests to include/exclude */
static rtest_t ** choose (char * progname, char * included [], char * excluded [])
{
  char ** names   = included ? included : excluded;
  rtest_t ** subset = included ? NULL : runit_all ();

  /* Nothing but these */
  while (names && * names)
    {
      rtest_t * runit = runit_valid (* names);
      if (! runit)
	{
	  printf ("%s: [%s] is not a valid id\n", progname, * names);
	  arrclear (subset, NULL);
	  return NULL;
	}
      else
	subset = included ? arrmore (subset, runit, rtest_t) : arrless (subset, runit, rtest_t, NULL);
      names ++;
    }

  return subset;
}


/* Attempt to do what has been required by the user */
static void doit (char * progname, unsigned choice, rtest_t * argv [],
		  unsigned items, unsigned runs, bool verbose, bool quiet)
{
  if (argv)
    {
      robj_t ** objs;
      switch (choice)
	{
	case OPT_RUN_UNIT: runit_run (argv, items);   break;
	case OPT_RUN_SUITE:
	  objs = mkobjs (items);
	  rsuite_run (argv, items, objs);
	  rmobjs (objs);
	  break;
	}
    }
  else
    printf ("No tests defined. Try '%s --help' for more information.\n", progname);
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

  printf ("  Operations on the Unit Tests and Suite:\n");
  usage_item (options, n, OPT_LIST_UNIT,    "list Unit Tests");
  usage_item (options, n, OPT_LIST_SUITE,   "list Suite");
  usage_item (options, n, OPT_RUN_UNIT,     "run Unit Tests");
  usage_item (options, n, OPT_RUN_SUITE,    "run Test Suite");
  printf ("\n");

  printf ("  Finger:\n");
  usage_item (options, n, OPT_INCLUDE,      "include item (repeatable)");
  usage_item (options, n, OPT_EXCLUDE,      "exclude item (repeatable)");
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


/* Display/Select/Run Unit Tests or Suite */
int main (int argc, char * argv [])
{
  char * progname  = basename (argv [0]);    /* notice program name */
  char * sopts     = optlegitimate (lopts);  /* short option list   */

  /* Booleans */
  bool verbose     = false;
  bool quiet       = false;

  /* Unit Tests */
  rtest_t ** units = NULL;
  rtest_t ** suite = NULL;
  char ** included = NULL;
  char ** excluded = NULL;

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
	  printf ("Try '%s --help' for more information.\n", progname); return 1;

	  /* Miscellanea */
	case OPT_HELP:       _usage_ (progname, _VERSION_, lopts); return 0;
	case OPT_VERSION:    _version_ (progname, _VERSION_);      return 0;
	case OPT_VERBOSE:    verbose = true;                       break;
	case OPT_QUIET:      quiet   = true;                       break;

	  /* Operations */
	case OPT_LIST_UNIT:  units = runit_all ();                 break;
	case OPT_LIST_SUITE: suite = rsuite_all ();                break;
	case OPT_RUN_UNIT:   choice = option;                      break;
	case OPT_RUN_SUITE:  choice = option;                      break;

	  /* Finger */
        case OPT_INCLUDE:    included = argsuniq (included, optarg); break;
        case OPT_EXCLUDE:    excluded = argsuniq (excluded, optarg); break;

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

  if (units)
    runit_print (units);
  else if (suite)
    rsuite_print (suite);
  else
    {
      rtest_t ** all;
      if (choice == OPT_RUN_UNIT)
	all = runit_all ();
      else if (choice == OPT_RUN_SUITE)
	all = rsuite_all ();

      /* Attempt to do what has been required by the user */
      if (included || excluded)
	{
	  /* Build a subset and go! */
	  rtest_t ** subset = choose (progname, included, excluded);
	  if (subset)
	    doit (progname, choice, subset, items, runs, verbose, quiet);
	  else
	    printf ("%s: Empty subset\n", progname);
	  arrclear (subset, NULL);
	}
      else
	doit (progname, choice, all, items, runs, verbose, quiet);
      arrclear (all, NULL);
    }

  /* Memory cleanup */
  argsclear (excluded);
  argsclear (included);
  arrclear (suite, NULL);
  arrclear (units, NULL);

  return 0;
}
