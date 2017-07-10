/* System headers */
#include <stdio.h>
#include <libgen.h>
#include <sys/utsname.h>
#include <getopt.h>


/* All the pluings to load at boot time */
static char * all [] =
{
  "/home/rocco/rperf/implementations/apr/apr.so",
  "/home/rocco/rperf/implementations/gcc/boost.so",
  "/home/rocco/rperf/implementations/google-c/dense/c-densehash.so",
  "/home/rocco/rperf/implementations/google-c/sparse/c-sparsehash.so",
  "/home/rocco/rperf/implementations/hashit/chain/hashit-chain.so",
  "/home/rocco/rperf/implementations/hashit/open/hashit-open.so",
  "/home/rocco/rperf/implementations/hashit/overflow/hashit-overflow.so",
  "/home/rocco/rperf/implementations/gcc/hashtab.so",
  "/home/rocco/rperf/implementations/klib/khash.so",
  "/home/rocco/rperf/implementations/libcfu/libcfu.so",

#if (0)
  /* need attention because it is not usable here and I do not know why */
  "/home/rocco/rperf/implementations/libevent/libevent.so",
#endif

  "/home/rocco/rperf/implementations/mct/closed/closed-hash-map.so",
  "/home/rocco/rperf/implementations/mct/linked/linked-hash-map.so",
  "/home/rocco/rperf/implementations/oddou/oddou.so",
  "/home/rocco/rperf/implementations/python/python.so",
  "/home/rocco/rperf/implementations/rdestl/rdestl.so",
  "/home/rocco/rperf/implementations/rigtorp/rigtorp.so",
  "/home/rocco/rperf/implementations/sys-boost/boost.so",
  "/home/rocco/rperf/implementations/sys-glib/ghash.so",
  "/home/rocco/rperf/implementations/sys-google/dense/densehash.so",
  "/home/rocco/rperf/implementations/sys-google/sparse/sparsehash.so",
  "/home/rocco/rperf/implementations/sys-libstdc++/unordered-map/unordered-map.so",

#if (0)
  /* need attention because these are not usable because of the bad glue implementation of rhash_get() via foreach */
  "/home/rocco/rperf/implementations/tommyds/dynamic/tommy-dyn.so",
  "/home/rocco/rperf/implementations/tommyds/linear/tommy-lin.so",
#endif

  "/home/rocco/rperf/implementations/ulib/chain/chain-hash.so",
  "/home/rocco/rperf/implementations/ulib/open/open-hash.so",

#if (0)
  /* need attention because it is not usable here and I do not know why */
  "/home/rocco/rperf/implementations/uthash/uthash.so",
#endif

#if (0)
  /* malloc version */
  "/home/rocco/rperf/implementations/z-experimental/libevent/libevent.so",
  "/home/rocco/rperf/implementations/z-experimental/tommy-dynamic/tommy-dyn.so",
  "/home/rocco/rperf/implementations/z-experimental/tommy-linear/tommy-lin.so",
  "/home/rocco/rperf/implementations/z-experimental/uthash/uthash.so",
#endif

  NULL
};


/* Public header */
#include "rtest.h"
#include "rbattle.h"


#define VERSION   "0.1.0"      /* program version  */


#define TOOSLOW   10    /* too slow condition limit            */
#define REPEAT    100   /* # of items to add for test repetion */
#define NMORE     500   /* # of items to add at the next run   */


/* All the command line long options are here */
typedef enum
{
  /* Miscellanea */
  OPT_HELP       = 'h',
  OPT_VERSION    = 'v',
  OPT_VERBOSE    = 'V',
  OPT_QUIET      = 'q',

  /* Item counters */
  OPT_ITEMS      = 'n',    /* initial # of items to play with */
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
  OPT_SLOW       = 's',
  OPT_REPEAT     = 't',
  OPT_MORE       = 'm',

  /* Display */
  OPT_LESS       = 'l',
  OPT_SHOW       = 'b',

} opt_t;


/* GNU Long Options */
static struct option long_options [] =
{
  /* Miscellanea */
  { "help",         no_argument,       NULL, OPT_HELP    },
  { "version",      no_argument,       NULL, OPT_VERSION },
  { "verbose",      no_argument,       NULL, OPT_VERBOSE },
  { "quiet",        no_argument,       NULL, OPT_QUIET   },

  /* Item counters */
  { "items",        required_argument, NULL, OPT_ITEMS   },     /* initial # of items */
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

  /* Run counters */
  { "runs",         required_argument, NULL, OPT_RUNS    },     /* # of runs per test */
  { "tooslow",      required_argument, NULL, OPT_SLOW    },     /* too slow condition limit */
  { "repeat",       required_argument, NULL, OPT_REPEAT  },     /* # of items to add for test repetion */
  { "more",         required_argument, NULL, OPT_MORE    },     /* # of items to add at the next run */

  /* Display */
  { "less",         no_argument,       NULL, OPT_LESS     },
  { "show",         no_argument,       NULL, OPT_SHOW     },

  /* End of options */
  { NULL,           0,                 NULL, 0           }
};


/* Display version information */
static void version (char * progname)
{
  printf ("%s version %s built on %s %s\n", progname, VERSION, __DATE__, __TIME__);
  fflush (stdout);
}


/* Display the syntax for using this program */
static void usage (char * progname, char * argv [], struct option * options)
{
  /* Get the longest option name for better rendering */
  unsigned n = optmax (options);

  printf ("%s %s, Benchmark program for Data Strutures\n", progname, VERSION);
  printf ("\n");

  printf ("Usage: %s [options]\n", progname);
  printf ("\n");

  printf ("  Miscellanea:\n");
  usage_item (options, n, OPT_HELP,    "Show this message and exit");
  usage_item (options, n, OPT_VERSION, "Show version information and exit");
  usage_item (options, n, OPT_VERBOSE, "Enable verbose mode");
  usage_item (options, n, OPT_QUIET,   "Enable quiet mode");
  printf ("\n");

  printf ("  Item counters: (default %.0f)\n", INITIALS);
  usage_item (options, n, OPT_ITEMS,   "Set the initial number of items per test");
  usage_item (options, n, OPT_ITEMS_0, "items set to one                  (1e0)");
  usage_item (options, n, OPT_ITEMS_1, "items set to ten                  (1e1");
  usage_item (options, n, OPT_ITEMS_2, "items set to one hundred          (1e2)");
  usage_item (options, n, OPT_ITEMS_3, "items set to one thousand         (1e3)");
  usage_item (options, n, OPT_ITEMS_4, "items set to ten thousand         (1e4)");
  usage_item (options, n, OPT_ITEMS_5, "items set to one hundred thousand (1e5)");
  usage_item (options, n, OPT_ITEMS_6, "items set to one million          (1e6)");
  usage_item (options, n, OPT_ITEMS_7, "items set to ten million          (1e7)");
  usage_item (options, n, OPT_ITEMS_8, "items set to one hundred million  (1e8)");
  usage_item (options, n, OPT_ITEMS_9, "items set to one billion          (1e9)");
  printf ("\n");

  printf ("  Run counters:\n");
  usage_item (options, n, OPT_RUNS,    "Set the number of runs per test (default 10)");
  usage_item (options, n, OPT_SLOW,    "Set the occurrences to mark an implementation too slow (default 10)");
  printf ("\n");

  usage_item (options, n, OPT_LESS,    "Enable less mode (do not show each test execution)");
  usage_item (options, n, OPT_SHOW,    "Enable verbose mode to show each test execution");
}


/* Run benchmarks */
int main (int argc, char * argv [])
{
  char * progname      = basename (argv [0]);             /* notice program name     */
  char * short_options = optlegitimate (long_options);    /* short option list       */

  /* Items counters */
  unsigned items  = INITIALS;                             /* initial # of items per test      */

  /* Run counters */
  unsigned runs   = RUNS;                                 /* # of run per test                */
  unsigned nslow  = TOOSLOW;                              /* too slow limit per test          */
  unsigned repeat = REPEAT;                               /* add # items (repetition)         */
  unsigned nmore  = NMORE;                                /* # of items to increment per test */

  /* Booleans */
  bool verbose    = false;
  bool less       = false;
  bool show       = true;

  struct utsname u;
  int option;
  sw_t ** running;

  while ((option = getopt_long (argc, argv, short_options, long_options, NULL)) != -1)
    {
      switch (option)
        {
	default: return 0;

	  /* Miscellanea */
	case OPT_HELP:      usage (progname, argv + 2, long_options);   return 0;
	case OPT_VERSION:   version (progname);                         return 0;
	case OPT_VERBOSE:   verbose = true;                             break;
	case OPT_QUIET:     verbose = false;                            break;

	  /* Item counters */
	case OPT_ITEMS:   items   = atoi (optarg); break; 	  /* initial # of items to play with */
	case OPT_ITEMS_0: items   = 1e0;           break;
	case OPT_ITEMS_1: items   = 1e1;           break;
	case OPT_ITEMS_2: items   = 1e2;           break;
	case OPT_ITEMS_3: items   = 1e3;           break;
	case OPT_ITEMS_4: items   = 1e4;           break;
	case OPT_ITEMS_5: items   = 1e5;           break;
	case OPT_ITEMS_6: items   = 1e6;           break;
	case OPT_ITEMS_7: items   = 1e7;           break;
	case OPT_ITEMS_8: items   = 1e8;           break;
	case OPT_ITEMS_9: items   = 1e9;           break;

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

  /* Avoid to run with 0 initial items */
  if (! items)
    items = INITIALS;

  if (! runs)
    runs = RUNS;

  if (! nslow)
    nslow = TOOSLOW;

  /* Set unbuffered stdout */
  setvbuf (stdout, NULL, _IONBF, 0);

  /* Welcome on board! */
  uname (& u);
  printf ("%s %s %s %s %s\n", u . sysname, u . nodename, u . release, u . version, u . machine);
  printf ("\n");

  /* Initialize all the implementations under test */
  running = sw_init (all, items, verbose);

  /* Run all tests for all the implementations to benchmark */
  run_all_tests (running, items, runs, nslow, repeat, nmore, verbose, less, show);

  /* Terminate all the implementations under test */
  sw_done (running, verbose);

  return 0;
}
