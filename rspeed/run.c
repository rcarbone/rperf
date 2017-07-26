/* Project headers */
#include "safe.h"
#include "args.h"
#include "rwall.h"
#include "rtest.h"
#include "rspeed.h"


/*
 * Run a single test.
 *
 * Iterate to run 'loops' times the same test and evaluate min/avg/max time spent
 */
static rspent_t * run_single_test (rtest_t * rtest, sw_t * sw,
				   unsigned items, robj_t * objs [],
				   unsigned serial, unsigned loops,
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
      double spent = sw_call (sw, rtest -> name, items, objs, false);

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

  /* Update the number of items used in this test */
  result -> items = items;

  /* Display timing information for this test */
  if (verbose)
    show_spent (result);

  return result;
}


/*
 * Run the given test _suite_ over the _plugins_ already loaded.
 *
 * 1. iterate over all the suite of tests (in the same order they were specified by the calling function)
 *  2. iterate over all loaded software implementations (in a randomized order) that have the test implemented
 *   3. iterate for the given number of _loops_ in order to repeat the same test and take the min/avg/max times spent
 *
 * The datasets needed to run the suite are initialized just one time because the number of items is not incremented each loop.
 */
sw_t ** run_suite (char * suite [], sw_t * plugins [],
		   unsigned initials, unsigned loops,
		   bool verbose, bool quiet)
{
  /* Initialize variables needed to run the suite */
  robj_t ** objs   = mkobjs (initials);         /* Initialize datasets needed by the suite */
  unsigned loaded  = arrlen (plugins);          /* Number of loaded implementations        */
  unsigned maxn    = sw_maxname (plugins);      /* Length of longest implementation name   */
  rtest_t ** tests = NULL;                      /* The table of tests to run               */
  unsigned n       = rsuite_maxn (tests);
  unsigned t;                                   /* Counter for tests to run                */
  rtest_t ** test;                              /* Iterator over the table of tests to run */
  char ** names;

  /* Lookup for the given names in the table of given test suite to run */
  names = suite;
  while (names && * names)
    tests = arrmore (tests, rsuite_find_by_name (* names ++), rtest_t);

  if (! suite || ! plugins || ! tests)
    return plugins;

  printf ("Evaluate average wall-time spent repeating %u times the same test with %u elements per test\n", loops, initials);
  printf ("\n");

  n = rsuite_maxn (tests);

  {
    unsigned d = rsuite_maxd (tests);

    printf ("Tests to run: %u using %u loaded implementations\n", arrlen (tests), arrlen (plugins));
    t = 0;
    test = tests;
    while (test && * test)
      {
	printf ("  %u: %-*.*s (%-*.*s)\n", ++ t, n, n, (* test) -> name, d, d, (* test) -> description);
	test ++;
      }
    printf ("\n");
  }

  /* Main loop - Iterate over all the names of the given test suite in the same order they were passed */
  t = 0;
  test = tests;
  while (test && * test)
    {
      unsigned torun = sw_have (plugins, (* test) -> name);          /* Implementations to run for this test */

      /* Run this test if there is at least 1 implementation that have the test implemented */
      if (torun)
	{
	  unsigned * order = rndorder (loaded);      /* Evaluate a random array to run implementations */
	  unsigned i;                                /* Iterator over the implementations              */

	  /* Display test information */
	  if (verbose)
	    print_test_info ("Running", (* test) -> name, initials, loops, maxn);
	  else
	    print_dots ((* test) -> name, "Running", 1, ++ t, n);

	  /*
	   * Inner loop over all the loaded implementations:
	   *  * run a single test for the given numer of times and take min/avg/max times spent for its execution
	   */
	  for (i = 0; i < loaded; i ++)
	    {
	      /* Select the implementation in random order */
	      sw_t * sw = plugins [order [i]];

	      /* Check if the current plugin implements the current test */
	      if (rplugin_implement (sw -> plugin, (* test) -> name))
		{
		  /* Run this test for this implementation using the same constant numer of items */
		  rspent_t * result = run_single_test (* test, sw, initials, objs, i + 1, loops, maxn, quiet);

		  /* Save the results for later sorting/rendering */
		  (* test) -> results = arrmore ((* test) -> results, result, rspent_t);
		}
	    }

	  /* Free the memory used by the test suite */
	  safefree (order);

	  /* Sort the results by less avg time spent */
	  (* test) -> results = arrsort ((* test) -> results, sort_by_less_avg, rspent_t);

	  /* Show the results */
	  if (verbose)
	    print_results ((* test) -> results, (* test) -> name, maxn, initials, loops);
	  else
	    printf ("Done\n");
	}
      test ++;
    }

  /* Display the results sorted by more performant application */
  hall_of_fame (suite, plugins, maxn, initials, loops);

  arrclear (tests, NULL);

  /* Free the datasets used by the test suite */
  rmobjs (objs);

  return plugins;
}
