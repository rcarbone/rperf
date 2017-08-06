/* Project headers */
#include "safe.h"
#include "sargs.h"
#include "rwall.h"
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


static void print_winner (relapsed_t * argv [], char * name, unsigned maxn, unsigned items, unsigned loops)
{
  relapsed_t ** rev = (relapsed_t **) varev ((void **) argv);
  print_results (rev, name, maxn, items, loops);
  vaclear ((void **) rev, NULL);
}


/* Initialize the table of runners for the test [name] */
static relapsed_t ** mkrunners (char * name, sw_t * plugins [])
{
  relapsed_t ** runners = NULL;

  while (plugins && * plugins)
    {
      if (rplugin_implement ((* plugins) -> plugin, name))
	runners = arrmore (runners, mkelapsed (* plugins), relapsed_t);
      plugins ++;
    }
  return runners;
}


/* Check if _name_ is included in the table of loosers [argv] */
static bool islooser (char * name, relapsed_t * argv [])
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


static unsigned getslow (char * name, relapsed_t * runners [])
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
 * Lookup for the worse implementation in [results] and in the event it is too sloow include it in the table of loosers
 */
static relapsed_t ** update_results (relapsed_t * results [],
				     relapsed_t * runners [], relapsed_t * loosers [],
				     unsigned nslow, unsigned rank, unsigned maxn, char * elapsed)
{
  relapsed_t * worse;
  sw_t * sw;

  /* Sort the results by more avg time elapsed in order to evalute worse implementation for this run */
  results = arrsort (results, sort_by_more_avg, relapsed_t);
  worse = results [0];
  sw = worse -> sw;

  /* Update the stop condition for the worse item */
  worse -> slow ++;

  /* Check now for the stop condition and in the event include the worse in the table of loosers */
  if (worse -> slow == nslow)
    {
      /* In the event include the worse in the table of loosers */
      loosers = arrmore (loosers, dupelapsed (worse), relapsed_t);

      /* Reset the stop condition for all the runners in order to repeat the suite with a clean environment */
      while (runners && * runners)
	(* runners ++) -> slow = 0;

      printf ("  battle for %2u-%s: %-*.*s      out of race after %s\n",
	      rank, rank == 2 ? "nd" : rank == 3 ? "rd" : "th",
	      maxn, maxn, sw -> name, elapsed);
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
 * Iterate to run 'loops' times the same test and evaluate min/avg/max time elapsed
 */
static relapsed_t * run_single_test (rtest_t * rtest, sw_t * sw,
				     unsigned loops,
				     unsigned items, robj_t * objs [],
				     unsigned serial,
				     unsigned slow,
				     unsigned maxn,
				     bool verbose)
{
  /* Allocate memory to store the results and bind the sw implementation to it */
  relapsed_t * result = mkelapsed (sw);
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
      /* Run the test and evaluate the time elapsed for this run in nanoseconds */
      double elapsed = sw_call (sw, rtest -> name, items, objs, false);

      /* Evaluate min/max/avg time elapsed for the execution of the test */
      min = RMIN (min, elapsed);
      max = RMAX (max, elapsed);
      avg += elapsed;
    }

  /* The wall time the test was completed at nsec resolution */
  result -> t2 = nswall ();

  /* Evaluate the time elapsed for the test */
  result -> elapsed = result -> t2 - result -> t1;

  /* Evaluate min/max/avg time elapsed for the execution of the test */
  result -> min = min;
  result -> max = max;
  result -> avg = avg;
  result -> avg /= loops;   /* normalize */

  /* Evaluate test rate */
  result -> rate = result -> elapsed / items / loops;

  /* Update the number of items used in this test */
  result -> items = items;

  /* Update the stop condition */
  result -> slow  = slow;

  /* Display timing information for this test */
  if (verbose)
    show_elapsed (result);

  return result;
}


/*
 * Run the given test _suite_ over the _plugins_ already loaded.
 *
 * 1. iterate over all the suite of tests (in the same order they were specified by the calling function)
 *  2. iterate over all loaded software implementations (in a randomized order) that have the test implemented
 *   3. iterate for the given number of _loops_ in order to repeat the same test and take the min/avg/max times elapsed
 *
 * The datasets needed to run the suite are initialized at each loop because the number of items is incremented each loop.
 */
sw_t ** run_suite (rtest_t * suite [], sw_t * plugins [],
		   unsigned loops, unsigned initials,
		   unsigned nslow, unsigned repeat, unsigned more,
		   bool verbose, bool quiet)
{
  /* Initialize variables needed to run the suite */
  unsigned loaded = arrlen (plugins);           /* Number of loaded implementations        */
  unsigned maxn   = sw_maxname (plugins);       /* Lenght of longest implementation name   */
  rtest_t ** test;                              /* Iterator over the table of tests to run */

  /* Nothing to do if no test or no plugins */
  if (! suite || ! plugins)
    return plugins;

  /*
   * Main loop:
   *   iterate over all the names of the given test suite in the same order they were passed
   */
  test = suite;
  while (test && * test)
    {
      unsigned items      = initials;                              /* Initial value of items for all tests             */
      unsigned torun      = sw_have (plugins, (* test) -> name);   /* Number of implementations to run for this test   */
      relapsed_t ** runners = mkrunners ((* test) -> name, plugins); /* The table of runners for this test               */
      relapsed_t ** loosers = NULL;                                  /* The table of loosers for this test               */

      /* Display test information */
      if (! quiet)
	print_test_info ((* test) -> name, torun, items, loops, more);
      else
	printf ("Running test [%s]. Please wait ...\n", (* test) -> name);

      /* Run this test if there is at least 2 implementations that have the test implemented */
      if (torun > 1)
	{
	  /* Initialize the table of runners for this test */
	  unsigned i;
	  for (i = 0; i < loaded; i ++)
	    {
	      sw_t * sw  = plugins [i];
	      if (rplugin_implement (sw -> plugin, (* test) -> name))
		runners = arrmore (runners, mkelapsed (sw), relapsed_t);
	    }

	  /*
	   * Inner loop:
	   *   continuosly repeat each single test per implementation until we have a winner.
	   */
	  while (arrlen (loosers) < torun - 1)
	    {
	      robj_t ** objs      = mkobjs (items);           /* Initialize the memory needed by the test suite */
	      unsigned * order    = rndorder (loaded);        /* Evaluate a random array to run implementation  */
	      relapsed_t ** results = NULL;
	      unsigned rank       = torun - arrlen (loosers);
	      unsigned tested     = 0;                        /* Counter over implementations tested            */
	      unsigned i;
	      rtime_t t1;
	      rtime_t t2;

	      /*
	       * Deep loop:
	       *   iterate over all loaded implementations 
	       *     run a single test in order and evaluate min/avg/max times elapsed for its execution
	       */
	      t1 = nswall ();
	      for (i = 0; i < loaded; i ++)
		{
		  /* Select the implementation under test in random order */
		  sw_t * sw  = plugins [order [i]];

		  /*
		   * In order to run a test for an implementation the following conditions must meet:
		   *  1. the implementation must have the test defined
		   *  2. the implementation must not be already included in the table of loosers
		   */
		  if (rplugin_implement (sw -> plugin, (* test) -> name) && ! islooser (sw -> name, loosers))
		    {
		      /* Run this test for this implementation with the current number of items */
		      relapsed_t * result = run_single_test (* test, sw, loops, items, objs,
							     ++ tested, getslow (sw -> name, runners), maxn, verbose);
		      if (! quiet)
			{
			  printf ("  battle for %2u-%s: %-*.*s ... %s",
				  rank, rank == 2 ? "nd" : rank == 3 ? "rd" : "th",
				  maxn, maxn, sw -> name, ns2a (result -> avg));

			  if (verbose)
			    printf ("\n");
			  else
			    printf ("\r");
			}

		      /* Save the results of current run for later evaluation/sort/rendering */
		      results = arrmore (results, result, relapsed_t);
		    }
		}
	      t2 = nswall ();

	      /* Update the table of runners/loosers for this test (table of results is sorted) */
	      loosers = update_results (results, runners, loosers, nslow, rank, maxn, ns2a (t2 - t1));

	      /* Check for winner */
	      if (arrlen (loosers) == torun - 1)
		{
		  printf ("  winner is  %2u: %s\n", 1, ((sw_t *) results [1] -> sw) -> name);
		  loosers = arrmore (loosers, dupelapsed (results [1]), relapsed_t);
		}

	      /* Free the memory used by the test suite */
	      arrclear (results, rmelapsed);
	      safefree (order);
	      rmobjs (objs);
	    }

	  /* Print the complete table of results for this test (in terms of ranking because times are less important) */
	  print_winner (loosers, (* test) -> name, maxn, items, loops);
	}

      /* Free the memory used by the test suite */
      arrclear (loosers, rmelapsed);
      arrclear (runners, rmelapsed);

      test ++;
      if (* test)
	printf ("\n");
    }

  return plugins;
}
