/* Public header */
#include "safe.h"
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
  rspent_t * result = mkspent ();
  double min = ULONG_MAX;
  double avg = 0;
  double max = 0;
  unsigned l;

  /* Bind the sw implementation result */
  result -> sw = sw;

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
 *  2. iterate over all loaded software implementations (in a randomized order)
 *   3. iterate for the given number of _loops_ in order to repeat the same test and take the times spent
 */
sw_t ** run_suite (char * suite [], sw_t * plugins [],
		   unsigned items, unsigned loops,
		   bool verbose, bool quiet)
{
  /* Initialize variables needed to run the suite */
  char ** names   = suite;
  robj_t ** objs  = mkobjs (items);            /* Initialize datasets needed by the suite */
  unsigned loaded = arrlen (plugins);          /* Number of loaded implementations        */
  unsigned maxn   = sw_maxname (plugins);      /* Lenght of longest implementation name   */
  unsigned t      = 0;                         /* Iterator over all the tests defined     */

  /* Iterate over all defined tests in the same order they were defined in the internal (static) table of tests */
  while (plugins && names && * names)
    {
      /* Lookup for the given name in the table of defined test suite */
      rtest_t * rtest = rsuite_find_by_name (* names);
      if (rtest && sw_have (plugins, * names))
	{
	  unsigned * order = rndorder (loaded);      /* Evaluate a random array to run implementations */
	  unsigned i;                                /* Iterator over the implementations              */

	  /* Display test information */
	  if (verbose)
	    {
	      /* Display test info */
	      print_test_info ("Running", * names, items, loops);

	      /* Display test header */
	      print_test_header (maxn);
	    }
	  else
	    printf ("Running test [%s]. Please wait ... ", * names);

	  /*
	   * Inner loop over all the loaded implementations:
	   *  * run a single test for the given numer of times and take the times spent for the its execution
	   */
	  for (i = 0; i < loaded; i ++)
	    {
	      /* Select the implementation in random order */
	      sw_t * sw  = plugins [order [i]];

	      if (rplugin_implement (sw -> plugin, * names))
		{
		  /* Run this test for this implementation */
		  rspent_t * result = run_single_test (rtest, sw, items, objs, i + 1, loops, maxn, quiet);

		  /* Save the results for later sorting/rendering */
		  rtest -> results = arrmore (rtest -> results, result, rspent_t);
		}
	    }

	  /* Free the memory needed by the test suite */
	  safefree (order);

	  /* Sort the results by less avg time spent */
	  rtest -> results = arrsort (rtest -> results, sort_by_less_avg, rspent_t);

	  /* Show the results */
	  if (verbose)
	    print_results (rtest -> results, * names, t, maxn, items, loops);
	  else
	    printf ("Done\n");

	  t ++;
	}
      names ++;
    }

  /* Display the results sorted by more performant application */
  hall_of_fame (suite, plugins, maxn, items, loops);

  /* Free the datasets used by the test suite */
  rmobjs (objs);

  return plugins;
}
