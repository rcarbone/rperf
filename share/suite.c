/* System headers */
#include <stdio.h>
#include <libgen.h>
#include <getopt.h>

/* Project headers */
#include "roptions.h"
#include "varrays.h"
#include "args.h"
#include "rtest.h"


/* Program name/version */
#define _NAME_      "Run builtin tests over this implementation"
#define _VERSION_   "0.1.0"

/* Default option (if none was specified) */
#define OPT_DEFAULT OPT_LIST


/* All the command line long options are here */
typedef enum
{
  /* Miscellanea */
  OPT_HELP         = 'h',
  OPT_VERSION      = 'v',
  OPT_VERBOSE      = 'V',
  OPT_QUIET        = 'q',

  /* Operations */
  OPT_LIST         = 'l',
  OPT_RUN          = 'x',

  /* Finger */
  OPT_INCLUDE      = 'i',    /* include item */
  OPT_EXCLUDE      = 'e',    /* exclude item */

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

  /* Loop counter */
  OPT_LOOPS        = 'r',

} ropt_t;


/* GNU Long Options */
static struct option lopts [] =
{
  /* Miscellanea */
  { "help",         no_argument,       NULL, OPT_HELP    },
  { "version",      no_argument,       NULL, OPT_VERSION },
  { "verbose",      no_argument,       NULL, OPT_VERBOSE },
  { "quiet",        no_argument,       NULL, OPT_QUIET   },

  /* Operations */
  { "list",         no_argument,       NULL, OPT_LIST    },
  { "run",          no_argument,       NULL, OPT_RUN     },

  /* Finger */
  { "include",      required_argument, NULL, OPT_INCLUDE },
  { "exclude",      required_argument, NULL, OPT_EXCLUDE },

  /* Item counters */
  { "items",        required_argument, NULL, OPT_ITEMS   },
  { "one",          no_argument,       NULL, OPT_ITEMS_0 },
  { "ten",          no_argument,       NULL, OPT_ITEMS_1 },
  { "hundred",      no_argument,       NULL, OPT_ITEMS_2 },
  { "thousand",     no_argument,       NULL, OPT_ITEMS_3 },
  { "10-thousand",  no_argument,       NULL, OPT_ITEMS_4 },
  { "100-thousand", no_argument,       NULL, OPT_ITEMS_5 },
  { "million",      no_argument,       NULL, OPT_ITEMS_6 },
  { "10-million",   no_argument,       NULL, OPT_ITEMS_7 },
  { "100-million",  no_argument,       NULL, OPT_ITEMS_8 },
  { "billion",      no_argument,       NULL, OPT_ITEMS_9 },

  /* Loop counter */
  { "loops",        required_argument, NULL, OPT_LOOPS   },

  /* End of options */
  { NULL,           0,                 NULL, 0           }
};


/* Build the tests to run */
static rtest_t ** build_tests (char * progname, char * included [], char * excluded [])
{
  char ** names    = included ? included : excluded;       /* User-included items have priority over user-excluded */
  rtest_t ** tests = included ? NULL : rsuite_all ();      /* The tests to run (nothing or everything but these)   */

  /* Loop over all defined tests to build the subset of user selected */
  while (names && * names)
    {
      /* Add/Delete the given test to/from the table of given tests to run */
      rtest_t * t = rsuite_valid (* names);
      if (t)
	tests = included ? arrmore (tests, t, rtest_t) : arrless (tests, t, rtest_t, NULL);
      else
	{
	  printf ("%s: [%s] is not a valid id\n", progname, * names);
	  arrclear (tests, NULL);
	  return NULL;
	}
      names ++;
    }
  return tests;
}


/* Attempt to do what has been required by the user */
static void doit (char * progname, unsigned choice,
		  rtest_t * tests [],
		  unsigned loops, unsigned items,
		  bool verbose, bool quiet)
{
  robj_t ** objs = NULL;
  unsigned l;
  switch (choice)
    {
    case OPT_LIST: rsuite_print_all (); break;

    case OPT_RUN:
      objs = mkobjs (items);
      for (l = 0; l < loops; l ++)
	rsuite_run (tests, items, objs);
      rmobjs (objs);
      break;
    }
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
  usage_item (options, n, OPT_HELP,    "show this message and exit");
  usage_item (options, n, OPT_VERSION, "show version information and exit");
  usage_item (options, n, OPT_VERBOSE, "run tests verbosely");
  usage_item (options, n, OPT_QUIET,   "run tests quietly");
  printf ("\n");

  printf ("  Operations on the tests:\n");
  usage_item (options, n, OPT_LIST,    "list tests");
  usage_item (options, n, OPT_RUN,     "run tests");
  printf ("\n");

  printf ("  Finger:\n");
  usage_item (options, n, OPT_INCLUDE, "include item (repeatable)");
  usage_item (options, n, OPT_EXCLUDE, "exclude item (repeatable)");
  printf ("\n");

  printf ("  Item counters: (default %.0f)\n", INITIALS);
  usage_item (options, n, OPT_ITEMS,   "set the initial number of items per test");
  usage_item (options, n, OPT_ITEMS_0, "one item                   (1e0)");
  usage_item (options, n, OPT_ITEMS_1, "ten items                  (1e1)");
  usage_item (options, n, OPT_ITEMS_2, "one hundred items          (1e2)");
  usage_item (options, n, OPT_ITEMS_3, "one thousand items         (1e3)");
  usage_item (options, n, OPT_ITEMS_4, "ten thousand items         (1e4)");
  usage_item (options, n, OPT_ITEMS_5, "one hundred thousand items (1e5)");
  usage_item (options, n, OPT_ITEMS_6, "one million items          (1e6)");
  usage_item (options, n, OPT_ITEMS_7, "ten million items          (1e7)");
  usage_item (options, n, OPT_ITEMS_8, "one hundred million items  (1e8)");
  usage_item (options, n, OPT_ITEMS_9, "one billion items          (1e9)");
  printf ("\n");

  printf ("  Loop counter: (default %u)\n", LOOPS);
  usage_item (options, n, OPT_LOOPS,    "set the number of loops per test");
}


/* Display/Select/Run Tests */
int main (int argc, char * argv [])
{
  char * progname  = basename (argv [0]);    /* notice program name */
  char * sopts     = optlegitimate (lopts);  /* short option list   */

  /* Booleans */
  bool verbose     = false;
  bool quiet       = false;

  /* Tests to run */
  char ** included = NULL;
  char ** excluded = NULL;
  rtest_t ** tests = NULL;

  /* Items counter */
  unsigned items   = INITIALS;               /* initial # of items per test */

  /* Loop counter */
  unsigned loops   = 1;                      /* # of loops per test         */

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
	case OPT_HELP:    _usage_ (progname, _VERSION_, lopts);   goto bye;
	case OPT_VERSION: _version_ (progname, _VERSION_);        goto bye;
	case OPT_VERBOSE: verbose = true;                         break;
	case OPT_QUIET:   quiet   = true;                         break;

	  /* Operations */
	case OPT_LIST:    choice = option;                        break;
	case OPT_RUN:     choice = option;                        break;

	  /* Finger */
        case OPT_INCLUDE: included = argsuniq (included, optarg); break;
        case OPT_EXCLUDE: excluded = argsuniq (excluded, optarg); break;

	  /* Item counter */
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

	  /* Loop counter */
	case OPT_LOOPS:   loops = atoi (optarg); break;
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

  if (! loops)
    loops = LOOPS;

  /* Build a subset and go! */
  tests = build_tests (progname, included, excluded);
  if (tests)
    doit (progname, choice, tests, loops, items, verbose, quiet);
  else
    printf ("%s: no test to run\n", progname);

 bye:

  /* Memory cleanup */
  arrclear (tests, NULL);
  argsclear (excluded);
  argsclear (included);

  return 0;
}
