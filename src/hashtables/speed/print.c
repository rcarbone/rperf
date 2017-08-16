/* Project headers */
#include "rspeed.h"


/* Display general test information */
void print_test_info (char * label, char * name, unsigned loops, unsigned items, unsigned maxn)
{
  if (items % 10)
    printf ("%s test [%s] - %u items (%u loops per test)\n", label, name, items, loops);
  else
    printf ("%s test [%s] - 1e%u items (%u loops per test)\n", label, name, digits (items) - 1, loops);

  /* Display test header */
  print_test_header (maxn);
}


/* Display times information spent in test execution */
void print_results (relapsed_t * results [], char * testname, unsigned maxn, unsigned loops, unsigned items)
{
  unsigned seq = 0;

  /* Display test information */
  printf ("\n");
  print_test_info ("Results of", testname, loops, items, maxn);

  while (results && * results)
    {
      sw_t * sw = (* results) -> sw;
      printf ("  %2d: %-*.*s ", ++ seq, maxn, maxn, sw -> name);
      show_elapsed (* results ++);
    }
}


/* Display information about all the test executed sorted by best implementation per test */
void hall_of_fame (rtest_t * suite [], sw_t * plugins [],
		   unsigned maxn, unsigned loops, unsigned items)
{
  /* Line separator */
  printf ("\n\n");
  printf ("%*.*s%s\n", 30, 30, " ", "H a l l   o f   F a m e");

  /* Sort the results by less avg time for better rendering */
  rsuite_sort_results (suite);

  while (suite && * suite)
    {
      if (sw_have (plugins, (* suite) -> name))
	{
	  print_results ((* suite) -> results, (* suite) -> name, maxn, loops, items);
	  if (! * suite)
	    printf ("\n");
	}
      suite ++;
    }
}


/* Eval the position of an implementation in the execution of a test */
static int rank (char * name, relapsed_t * results [])
{
  unsigned n = 0;
  while (results && * results)
    {
      if (! strcmp (name, ((* results) -> sw) -> name))
	return n + 1;
      n ++;
      results ++;
    }
  return -1;
}


/* Evaluate the quality of an implementation after all the suite have been executed */
static unsigned eval_mark (sw_t * sw, rtest_t * suite [])
{
  unsigned mark = 0;

  /* Each implementation has its own table of results (that here should be already sorted) */
  rtest_t ** tests = suite;
  while (tests && * tests)
    {
      /* Number of tests executed over this implementation */
      unsigned tno = arrlen ((* tests) -> results);
      int r = rank (sw -> name, (* tests) -> results);
      if (r != -1)
	mark += tno - rank (sw -> name, (* tests) -> results) + 1;

      tests ++;
    }
  return mark;
}


/* Sort test results by avg time elapsed */
static int sort_by_mark (const void * a, const void * b)
{
  return (* (sw_t **) b) -> mark - (* (sw_t **) a) -> mark;
}


/* Print the table of graduation */
void print_ranking (rtest_t * suite [], sw_t * plugins [], unsigned maxn)
{
  char ** names = rsuite_names (suite);
  char ** name  = names;
  sw_t ** sw;                           /* iterator over all the implementations */
  unsigned seq;

  /* Sort the results by less avg time for good evaluation of order arrival */
  rsuite_sort_results (suite);

  /* Update evaluation of performances */
  sw = plugins;
  while (sw && * sw)
    {
      /* Evaluate the mark to be assigned to the implementations based on the results of the executions of the tests */
      (* sw) -> mark = eval_mark (* sw, suite);
      sw ++;
    }

  /* Line separator */
  printf ("\n\n");

  printf ("%*.*s%s\n", 30, 30, " ", "A r r i v a l   O r d e r");
  printf ("\n");

  printf ("      %-*.*s | mark ", maxn, maxn, "Implementation");
  seq = 0;
  name  = names;
  while (name && * name ++)
    printf ("| #%2u ", ++ seq);
  printf ("|\n");

  /* Sort the table of implementations accordingly to the evaluated quality */
  plugins = arrsort (plugins, sort_by_mark, sw_t);

  /* Main loop over all implementations under test */
  seq = 0;
  sw = plugins;
  while (sw && * sw)
    {
      /* The table of tests for this implementation */
      rtest_t ** tests = suite;

      if ((* sw) -> mark)
	{
	  printf ("%4u: %-*.*s | %4u |", ++ seq, maxn, maxn, (* sw) -> name, (* sw) -> mark);

	  while (tests && * tests)
	    {
	      /* Find the position of the test in the table of results */
	      char fmt [1024];
	      char str [1014];

	      if (rplugin_implement ((* sw) -> plugin, (* tests) -> name))
		{
		  sprintf (fmt, " %%%uu |", 3);
		  sprintf (str, fmt, rank ((* sw) -> name, (* tests) -> results));
		}
	      else
		{
		  sprintf (fmt, " %%%us |", 3);
		  sprintf (str, fmt, "---");
		}
	      printf ("%s", str);

	      tests ++;
	    }
	}

      if ((* sw) -> mark)
	printf ("\n");

      sw ++;
    }
}
