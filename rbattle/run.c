/* Project headers */
#include "safe.h"
#include "rtest.h"
#include "rbattle.h"


static char * ee (unsigned items)
{
  static char buf [128];

  if (items % 10)
    sprintf (buf, "%u", items);
  else
    sprintf (buf, "1e%u", digits (items) -1);
  return buf;  
}


/* Display test header */
static void print_test_info (char * name, unsigned swno, unsigned items, unsigned loops, unsigned more)
{
  printf ("Running test [%s] - %u implementations - %s initial items %u loops (+%u) ...\n",
	  name, swno, ee (items), loops, more);
}


static void print_winner (rspent_t * argv [], char * name, unsigned maxn, unsigned items, unsigned loops)
{
  rspent_t ** rev = (rspent_t **) varev ((void **) argv);
  print_results (rev, name, maxn, items, loops);
  vaclear ((void **) rev, NULL);
}


/* Initialize the table of runners for the test _name_ */
static rspent_t ** mkrunners (char * name, sw_t * plugins [])
{
  rspent_t ** runners = NULL;

  while (plugins && * plugins)
    {
      if (rplugin_implement ((* plugins) -> plugin, name))
	runners = arrmore (runners, mkspent (* plugins), rspent_t);
      plugins ++;
    }
  return runners;
}


/* Check if _name_ is included in the table of loosers _argv_ */
static bool islooser (char * name, rspent_t * argv [])
{
  while (name && argv && * argv)
    {
      sw_t * sw = (* argv) -> sw;
      if (sw && sw -> name && ! strcmp (name, sw -> name))
	return true;
      argv ++;
    }
  return false;
}


static unsigned getslow (char * name, rspent_t * runners [])
{
  while (runners && * runners)
    {
      sw_t * sw = (* runners) -> sw;
      if (sw && sw -> name && ! strcmp (name, sw -> name))
	return (* runners) -> slow;
      runners ++;
    }
  return 0;
}


/*
 * Lookup for a too slow implementation (if any) and in the event include it the table of loosers
 */
static rspent_t ** update_results (rspent_t * results [], rspent_t * runners [], rspent_t * loosers [], unsigned nslow, unsigned rank)
{
  rspent_t * worse;
  sw_t * sw;

  /* Sort the results by more avg time spent in order to evalute worse implementation for this run */
  results = arrsort (results, sort_by_more_avg, rspent_t);
  worse = results [0];
  sw = worse -> sw;

  /* Update the stop condition for the worse item */
  worse -> slow ++;

  /* Check now for the stop condition and in the event include the worse in the table of loosers */
  if (worse -> slow == nslow)
    {
      /* In the event include the worse in the table of loosers */
      loosers = arrmore (loosers, dupspent (worse), rspent_t);

      /* Reset the stop condition for all the runners in order to repeat the suite with a clean environment */
      while (runners && * runners)
	(* runners ++) -> slow = 0;

      printf ("  battle for %2u: %s too slow                                    \n", rank, sw -> name);
    }
  else
    {
      /* Update only the stop condition for worse implementation in the table of runners */
      while (runners && * runners)
	{
	  sw_t * sw1 = (* runners) -> sw;
	  sw_t * sw2 = worse -> sw;
	  if (sw1 && sw2 && sw1 -> name && sw2 -> name && ! strcmp (sw1 -> name, sw2 -> name))
	    {
	      (* runners) -> slow ++;
	      break;
	    }
	  runners ++;
	}
    }

  return loosers;
}


/*
 * Run a single test.
 *
 * Iterate to run 'loops' times the same test and evaluate min/avg/max time spent
 */
static rspent_t * run_single_test (char * name, sw_t * sw,
				   unsigned items, robj_t * objs [],
				   unsigned serial, unsigned loops, unsigned slow,
				   unsigned maxn, bool verbose)
{
  /* Allocate memory to store the results and bind the sw implementation to it */
  rspent_t * result = mkspent (sw);
  double min = ULONG_MAX;
  double avg = 0;
  double max = 0;
  unsigned l;

  /* Print the name of the implementation under test */
  if (verbose)
    printf ("  %2u: %-*.*s ", serial, maxn, maxn, sw -> name);

  /* The wall time the test was initiated at nsec resolution */
  result -> t1 = nswall ();

  /* Main loop - iterate to run test 'loops' times and evaluate min/max/avg */
  for (l = 0; l < loops; l ++)
    {
      /* Run the test and evaluate the time spent for this run in nanoseconds */
      double spent = sw_call (sw, name, items, objs, false);

      /* Evaluate min/max/avg time spent for the execution of the test */
      min = RMIN (min, spent);
      max = RMAX (max, spent);
      avg += spent;
    }

  /* The wall time the test was completed at nsec resolution */
  result -> t2 = nswall ();

  /* Evaluate the time spent for the test */
  result -> elapsed = result -> t2 - result -> t1;

  /* Evaluate min/max/avg time spent for the execution of the test */
  result -> min = min;
  result -> max = max;
  result -> avg = avg;
  result -> avg /= loops;   /* normalize */

  /* Evaluate test rate */
  result -> rate = result -> elapsed / items / loops;

  /* Update the number of items used and the stop condition */
  result -> items = items;
  result -> slow  = slow;

  /* Display timing information for this test */
  if (verbose)
    show_spent (result);

  return result;
}


/*
 *  tests    sw1  sw2
 *         +=====+=====+
 *  grow   |     |     |
 *         | === | === |
 *  hit    |     |     |
 *         | === | === |
 * ....... |     |     |
 *         +=====+=====+
 */

/*
 * Run the given test _suite_ over the _plugins_ already loaded.
 *
 * 1. iterate over all the suite of tests (in the same order they were specified by the calling function)
 *  2. iterate over all loaded plugin implementations (in a randomized order) that have the test implemented
 *   3. iterate for the given number of _loops_ in order to repeat the same test and take the min/avg/max times spent
 *
 * The datasets needed to run the suite are initialized at each loop because the number of items is incremented per loop
 */
sw_t ** run_suite (char * suite [], sw_t * plugins [],
		   unsigned initials, unsigned loops,
		   unsigned nslow, unsigned repeat, unsigned more,
		   bool verbose, bool quiet, bool less, bool show)
{
  /* Initialize variables needed to run the suite */
  char ** names   = suite;
  unsigned loaded = arrlen (plugins);                     /* Number of loaded implementations      */
  unsigned maxn   = sw_maxname (plugins);                 /* Lenght of longest implementation name */

  /*
   * Main loop
   *   Iterate over all the names of the given test suite in the same order they were passed
   */
  while (plugins && names && * names)
    {
      /* Lookup for the given name in the table of given test suite */
      rtest_t * rtest     = rsuite_find_by_name (* names); /* Lookup for given name in the table of test suite */
      unsigned items      = initials;                      /* Initial value of items for all tests             */
      unsigned torun      = sw_have (plugins, * names);    /* Number of implementations to run for this test   */
      rspent_t ** runners = mkrunners (* names, plugins);  /* The table of runners for this test               */
      rspent_t ** loosers = NULL;                          /* The table of loosers for this test               */
      unsigned rank       = torun;

      /* Display test information */
      print_test_info (* names, torun, items, loops, more);

      /* Run this test if there is at least 2 implementations that have the test implemented */
      if (rtest && torun > 1)
	{
	  /* Initialize the table of runners for this test */
	  unsigned i;
	  for (i = 0; i < loaded; i ++)
	    {
	      sw_t * sw  = plugins [i];
	      if (rplugin_implement (sw -> plugin, * names))
		runners = arrmore (runners, mkspent (sw), rspent_t);
	    }

	  /*
	   * Inner loop:
	   *   continuosly repeat each single test per implementation until we have a winner.
	   *
	   * In order to run a test for an implementation the following conditions must match:
	   *  1. the implementation must have the test defined
	   *  2. the implementation must not be already included in the table of loosers
	   */
	  while (arrlen (loosers) < torun - 1)
	    {
	      robj_t ** objs   = mkobjs (items);          /* Initialize the memory needed by the test suite */
	      unsigned * order = rndorder (loaded);       /* Evaluate a random array to run implementation  */
	      unsigned tested  = 0;                       /* Counter over implementations tested            */
	      rspent_t ** results = NULL;
	      unsigned i;

	      rank = torun - arrlen (loosers);

	      /*
	       * Deep loop - iterate over all loaded implementations
	       *   run a single test and take min/avg/max times spent for its execution
	       */
	      for (i = 0; i < loaded; i ++)
		{
		  /* Select the implementation under test in random order */
		  sw_t * sw  = plugins [order [i]];

		  /* Check if the current plugin implements the current test and it is not already between the loosers */
		  if (rplugin_implement (sw -> plugin, * names) && ! islooser (sw -> name, loosers))
		    {
		      /* Run this test for this implementation with the current number of items */
		      rspent_t * result = run_single_test (rtest -> name, sw, items, objs,
							   tested ++, loops, getslow (sw -> name, runners), maxn, quiet);

		      printf ("  battle for %2u: %-*.*s ... %s",
			      rank,
			      maxn, maxn, sw -> name,
			      ns2a (result -> avg));

		      if (verbose)
			printf ("\n");
		      else
			printf ("\r");

		      /* Save the results of current run for later evaluation/sort/rendering */
		      results = arrmore (results, result, rspent_t);
		    }
		}

	      /* Update the table of runners/loosers for this test (table of results is sorted) */
	      loosers = update_results (results, runners, loosers, nslow, rank);

	      if (arrlen (loosers) == torun - 1)
		{
		  printf ("  winner is  %2u: %s\n", 1, ((sw_t *) results [1] -> sw) -> name);
		  loosers = arrmore (loosers, dupspent (results [1]), rspent_t);
		}

	      /* Free the memory used by the test suite */
	      arrclear (results, rmspent);
	      safefree (order);
	      rmobjs (objs);
	    }

	  /* Print the complete table of results for this test (in terms of ranking) */
	  print_winner (loosers, * names, maxn, items, loops);
	}
      arrclear (loosers, rmspent);
      arrclear (runners, rmspent);
      names ++;

      if (* names)
	printf ("\n");
    }

  return plugins;
}
