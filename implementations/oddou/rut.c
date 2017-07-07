/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <getopt.h>

/* Project headers */
#include "rut.h"
#include "roptions.h"
#include "args.h"
#include "varrays.h"


/* Program version  */
#define _VERSION_  "0.1.0"

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
  OPT_EXECUTE      = 'x',

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

  /* Operations */
  { "list",         no_argument,       NULL, OPT_LIST         },
  { "execute",      no_argument,       NULL, OPT_EXECUTE      },

  /* Finger */
  { "include",      required_argument, NULL, OPT_INCLUDE      },
  { "exclude",      required_argument, NULL, OPT_EXCLUDE      },

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


/* Display the syntax for using this program */
static void _usage_ (char * progname, char * version, struct option * options)
{
  /* Get the longest option name for optimal rendering */
  unsigned n = optmax (options);

  printf ("%s %s, Unit Tests\n", progname, version);
  printf ("\n");

  printf ("Usage: %s [options]\n", progname);
  printf ("\n");

  printf ("  Miscellanea:\n");
  usage_item (options, n, OPT_HELP,         "show this message and exit");
  usage_item (options, n, OPT_VERSION,      "show version information and exit");
  usage_item (options, n, OPT_VERBOSE,      "run tests verbosely");
  usage_item (options, n, OPT_QUIET,        "run tests quietly");
  printf ("\n");

  printf ("  Operations with the Unit Tests:\n");
  usage_item (options, n, OPT_LIST,         "list items");
  usage_item (options, n, OPT_EXECUTE,      "execute items");
  printf ("\n");

  printf ("  Select:\n");
  usage_item (options, n, OPT_INCLUDE,      "include item (repeatable)");
  usage_item (options, n, OPT_EXCLUDE,      "exclude item (repeatable)");
  printf ("\n");

  printf ("  Item counters: (default %.0f)\n", INITIALS);
  usage_item (options, n, OPT_ITEMS,        "set the initial number of items per test");
  usage_item (options, n, OPT_ITEMS_0,      "one item                   (1e0)");
  usage_item (options, n, OPT_ITEMS_1,      "ten items                  (1e1");
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


/* Display version information */
static void _version_ (char * progname, char * version)
{
  printf ("%s version %s built on %s %s\n", progname, version, __DATE__, __TIME__);
  fflush (stdout);
}


/* Finger at Unit Tests to include/exclude */
static rut_t ** choose (char * progname, char * included [], char * excluded [])
{
  char ** names   = included ? included : excluded;
  rut_t ** subset = included ? NULL : rut_all ();

  /* Nothing but these */
  while (names && * names)
    {
      rut_t * rut = rut_valid (* names);
      if (! rut)
	{
	  printf ("%s: [%s] is not a valid id\n", progname, * names);
	  arrclear (subset, NULL);
	  return NULL;
	}
      else
	subset = included ? arrmore (subset, rut, rut_t) : arrless (subset, rut, rut_t, NULL);
      names ++;
    }

  return subset;
}


/* Attempt to do what has been required by the user */
static void doit (char * progname, unsigned choice, rut_t * rargv [], unsigned items, unsigned runs, bool verbose, bool quiet)
{
  if (rargv)
    {
      switch (choice)
	{
	case OPT_LIST:    rut_print_these (rargv);      break;
	case OPT_EXECUTE: rut_run_these (rargv, items); break;
	}
    }
  else
    printf ("%s: no Unit Tests defined\n", progname);
}


/* Display/Select/Execute Unit Tests */
int main (int argc, char * argv [])
{
  char * progname  = basename (argv [0]);    /* notice program name */
  char * sopts     = optlegitimate (lopts);  /* short option list   */

  /* Booleans */
  bool verbose     = false;
  bool quiet       = false;

  /* Unit Tests */
  rut_t ** all     = rut_all ();
  char ** included = NULL;
  char ** excluded = NULL;

  /* Items counters */
  unsigned items   = INITIALS;

  /* Run counters */
  unsigned runs    = RUNS;                  /* # of run per test   */

  unsigned choice  = OPT_DEFAULT;
  int option;

  /* Set unbuffered stdout */
  setvbuf (stdout, NULL, _IONBF, 0);

  /* Parse command-line options */
  while ((option = getopt_long (argc, argv, sopts, lopts, NULL)) != -1)
    {
      switch (option)
	{
	default:
	  printf ("%s: Unknown option -%c\n\n", progname, option);
	  _usage_ (progname, _VERSION_, lopts);                        return 1;

	  /* Miscellanea */
	case OPT_HELP:         _usage_ (progname, _VERSION_, lopts);   return 0;
	case OPT_VERSION:      _version_ (progname, _VERSION_);        return 0;
	case OPT_VERBOSE:      verbose = true;                         break;
	case OPT_QUIET:        quiet   = true;                         break;

	  /* Operations */
	case OPT_LIST:         choice = option;                        break;
	case OPT_EXECUTE:      choice = option;                        break;

	  /* Finger */
        case OPT_INCLUDE:      included = argsuniq (included, optarg); break;
        case OPT_EXCLUDE:      excluded = argsuniq (excluded, optarg); break;

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

  /* Attempt to do what has been required by the user */
  if (included || excluded)
    {
      rut_t ** subset = choose (progname, included, excluded);
      if (subset)
	{
	  doit (progname, choice, subset, items, runs, verbose, quiet);
	  arrclear (subset, NULL);
	}
      else
	printf ("%s: Empty subset\n", progname);
    }
  else
    doit (progname, choice, all, items, runs, verbose, quiet);

  /* Memory cleanup */
  arrclear (excluded, free);
  arrclear (included, free);
  arrclear (all, NULL);

  return 0;
}
