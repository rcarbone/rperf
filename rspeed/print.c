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
void print_results (rspent_t * results [], char * testname, unsigned maxn, unsigned loops, unsigned items)
{
  unsigned n = 0;

  /* Display test information */
  printf ("\n");
  print_test_info ("Results of", testname, items, loops, maxn);

  while (results && * results)
    {
      sw_t * sw = (* results) -> sw;
      printf ("  %2d: %-*.*s ", ++ n, maxn, maxn, sw -> name);
      show_spent (* results ++);
    }
}


/* Display information about all the test executed sorted by best implementation per test */
void hall_of_fame (rtest_t * suite [], sw_t * plugins [],
		   unsigned maxn, unsigned loops, unsigned items)
{
  /* Line separator */
  printf ("\n");
  printf ("%*.*s%s\n", 30, 30, " ", "H a l l   o f   F a m e");

  while (suite && * suite)
    {
      if (sw_have (plugins, (* suite) -> name))
	{
	  print_results ((* suite) -> results, (* suite) -> name, maxn, loops, items);
	  arrclear ((* suite) -> results, rmspent);
	  if (! * suite)
	    printf ("\n");
	}
      suite ++;
    }
}
