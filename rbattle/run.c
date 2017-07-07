/* Public header */
#include "rbattle.h"


/* Run a single test - Iterate to run 'loops' times the same test and evaluate min/avg/max time spent */
static void run_single_test (sw_t * sw, test_t * test,
                             unsigned items, robj_t * objs [],
                             unsigned i, unsigned n, unsigned loops,
			     unsigned maxn, bool verbose)
{
  unsigned l;

  double min = ULONG_MAX;
  double avg = 0;
  double max = 0;

  /* Print the name of the implementation under test */
  if (verbose)
    printf ("  %2u: %-*.*s ... ", i + 1, maxn, maxn, sw -> name);

  /* The wall time the test was initiated at nsec resolution */
  test -> t1 = nswall ();

  /* Main loop - iterate to run test */
  for (l = 0; l < loops; l ++)
    {
      double t1;
      double t2;
      double elapsed;

      /* Run the test and evaluate the time spent for this run in nanoseconds */
      t1 = nswall ();
      sw_call (sw, test -> name, items, objs, verbose);
      t2 = nswall ();

      /* Evaluate the time spent for the test */
      elapsed = t2 - t1;

      /* Evaluate min/max/avg time spent for the execution of the test */
      min = MIN (min, elapsed);
      max = MAX (max, elapsed);
      avg += elapsed;
    }

  /* The wall time the test was completed at nsec resolution */
  test -> t2 = nswall ();

  /* Evaluate the time spent for the test */
  test -> elapsed = test -> t2 - test -> t1;

  /* Evaluate min/max/avg time spent for the execution of the test */
  test -> min = min;
  test -> max = max;
  test -> avg = avg;
  test -> avg /= loops;   /* normalize */

  /* Evaluate test rate */
  test -> rate = test -> elapsed / n / loops;

  /* Update the number of items used in this test */
  test -> items = n;

  if (verbose)
    printf ("Ok! ");

  /* Display timing information for this test */
  if (verbose)
    old_show_info (sw, test, maxn);
}


/* Evaluate the number of implementations not yet considered too slow for a given test */
static unsigned eval_torun (sw_t * sw [], unsigned id, unsigned nslow)
{
  unsigned n = 0;
  test_t * t;
  while (sw && * sw)
    if ((t = get_test (* sw ++, id)) && t -> slow < nslow)
      n ++;
  return n;
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
 * Run all the tests
 *
 * 1. iterate over all the known kind of tests (in the same order they were defined in the internal static table of tests)
 *  2. iterate over all loaded software implementations (in a randomized order)
 */
sw_t ** run_all_tests (sw_t * sw [],
		       unsigned initials, unsigned ntry, unsigned nslow,
		       unsigned repeat, unsigned more,
		       bool verbose, bool less, bool show)
{
  unsigned all;           /* the number of loaded implementations */
  unsigned maxn;
  result_t ** results;    /* Get memory to store all the results */
  unsigned t;             /* iterator over the tests */

  if (! sw)
    return sw;

  all     = sw_no (sw);                /* the number of loaded implementations */
  maxn    = sw_maxname (sw);
  results = mkmatrix (tno (), all);    /* Get memory to store all the results */

  printf ("\n");

  /* Iterate over all defined tests in the same order they were defined in the internal static table of tests */
  for (t = 0; t < tno (); t ++)
    {
      unsigned items = initials;                     /* Initial value of items for all tests */
      unsigned done  = all - 1;
      unsigned torun = implemented (sw, t);          /* Implementations to run for this test */

      /* Run this test if there is at least 2 implementations that have the same test defined */
      if (torun > 1)
	{
	  result_t * partial = calloc (all, sizeof (result_t));   /* Get memory to save partial results */
	  unsigned k;                                             /* Iterator over the implementations  */
	  unsigned j;

	  /* Display test information */
	  if (items % 10)
	    printf ("Running test [%s] - %u initial items (+%u) %u loops ...\n", tname (t), items, more, ntry);
	  else
	    printf ("Running test [%s] - 1e%u initial items (+%u) %u loops ...\n", tname (t), digits (items) - 1, more, ntry);

	  /* Main loop to run each single test per implementation until we have a winner */
	  while (eval_torun (sw, t, nslow) > 1)
	    {
	      /* Allocate the memory needed by the test suite */
	      robj_t ** objs   = mkobjs (items);       /* Initialize the memory needed by the test suite */
	      unsigned * order = rndorder (all);       /* Evaluate a random array to run implementation  */
	      unsigned tested  = 0;                    /* Counter over implementations tested            */
	      unsigned i;                              /* Iterator over the implementations              */

	      /* Inner loop for all the known implementations */
	      for (i = 0; i < all; i ++)
		{
		  /* Select the implementation in random order and get the test (if any) */
		  sw_t * which  = sw [order [i]];
		  test_t * test = get_test (which, t);

		  if (test && test -> slow < nslow)
		    {
		      /* Run the test */
		      run_single_test (which, test, items, objs, tested + 1, items, ntry, maxn, verbose);

		      partial [tested] . sw   = which;
		      partial [tested] . test = test;

		      tested ++;
		    }
		}

	      /* Free the memory needed by the test suite */
	      rmobjs (objs);
	      free (order);

	      /* Sort the results by less time spent */
	      qsort (partial, tested, sizeof (result_t), sort_by_less_spent);

	      /* Lookup for a too slow implementation (if any) */
	      for (i = 0; i < tested; i ++)
		if (partial [i] . test && partial [i] . test -> slow < nslow)
		  {
		    /* Found! Update the slow condition */
		    partial [i] . test -> slow ++;

		    if (partial [i] . test -> slow == nslow)
		      {
			/* Save the results */
			results [t] [done] . sw   = partial [i] . sw;
			results [t] [done] . test = partial [i] . test;

			done --;

#if defined(ROCCO)
			/* In such event increase number of items for the next run */
			items += (more - repeat);
#else
			items = initials;
#endif /* ROCCO */
		      }
		    break;
		  }

	      /* Reset the slow condition for all the other implementations */
	      for (j = i + 1; j < tested; j ++)
		if (partial [j] . test && partial [j] . test -> slow < nslow)
		  partial [j] . test -> slow = 0;

	      /* Print the partial results */
	      if (less)
		{
		  /* Implementation - current slow value - average time in ms - items */
		  printf ("  %2u: %-*.*s [%2u] - %-8u - %-10.10s",
			  torun,
			  maxn, maxn, partial [i] . sw -> name,
			  partial [i] . test -> slow,
			  partial [i] . test -> items,
			  ns2a (partial [i] . test -> elapsed));

		  if (partial [i] . test -> slow == nslow)
		    printf ("  %-*.*s too slow - limit %u reached at %u items\n",
			    maxn, maxn, partial [i] . sw -> name,
			    nslow,
			    partial [i] . test -> items);
		  else
		    printf ("\n");
		}
	      else if (partial [i] . test -> slow == nslow)
		printf ("             %2u: %-*.*s [%2u] - %-8u - %10.10s  too slow - limit %u reached at %u items\n",
			torun --,
			maxn, maxn, partial [i] . sw -> name,
			partial [i] . test -> slow,
			partial [i] . test -> items,
			ns2a (partial [i] . test -> elapsed),
			nslow,
			partial [i] . test -> items);
	      else if (show)
		printf ("  battle for %2u: %-*.*s [%2u] - %u ... +%u\r",
			torun,
			maxn, maxn, partial [i] . sw -> name,
			partial [i] . test -> slow,
			partial [i] . test -> items,
			repeat);

	      /* Increase number of items for the next run */
	      items += repeat;

	      if (verbose)
		printf ("\n");
	    }

	  /* And the winner is ... */
	  for (k = 0; k < all; k ++)
	    if (partial [k] . test && partial [k] . test -> slow == 0)
	      {
		printf ("             %2u: %-*.*s [%2u] - %-8u - %10.10s  %s  winner\n",
			torun,
			maxn, maxn, partial [k] . sw -> name,
			partial [k] . test -> slow,
			partial [k] . test -> items,
			ns2a (partial [k] . test -> elapsed),
			partial [k] . sw -> name);
		printf ("\n");

		results [t] [done] . sw = partial [k] . sw;
		results [t] [done] . test = partial [k] . test;

		done --;
		break;
	      }

	  free (partial);

	  /* Show the results */
	  if (show)
	    hall_of_fame (t, results, all, maxn, initials, repeat, more);
	}
    }

  printf ("\n");

  /* Best implementation per test */
  printf ("%*.*sH a l l   o f   F a m e\n", 30, 30, " ");

  /* Best implementation per test */
  for (t = 0; t < tno (); t ++)
    if (implemented (sw, t) > 1)
      hall_of_fame (t, results, all, maxn, initials, repeat, more);

  rmmatrix (results, tno ());

  return sw;
}
