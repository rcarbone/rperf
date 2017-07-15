/* Project headers */
#include "rspeed.h"


/* Display test information */
void print_test_info (char * label, char * name, unsigned items, unsigned loops)
{
  if (items % 10)
    printf ("%s test [%s] - %u items (%u loops per test)\n", label, name, items, loops);
  else
    printf ("%s test [%s] - 1e%u items (%u loops per test)\n", label, name, digits (items) - 1, loops);
}


/* Best implementation per test */
void print_results (rspent_t * results [], char * testname, unsigned maxn, unsigned items, unsigned loops)
{
  unsigned n = 0;

  /* Display test information */
  printf ("\n");
  print_test_info ("Results of", testname, items, loops);

  /* Display test header */
  print_test_header (maxn);

  while (results && * results)
    {
      sw_t * sw = (* results) -> sw;
      printf ("  %2d: %-*.*s ", ++ n, maxn, maxn, sw -> name);
      show_spent (* results ++);
    }
}


/* Display the best implementation per test */
void hall_of_fame (char * suite [], sw_t * plugins [],
		   unsigned maxn, unsigned items, unsigned loops)
{
  /* Line separator */
  printf ("\n");
  printf ("%*.*s%s\n", 30, 30, " ", "H a l l   o f   F a m e");

  while (suite && * suite)
    {
      if (sw_have (plugins, * suite))
	{
	  rtest_t * rtest = rsuite_find_by_name (* suite);
	  print_results (rtest -> results, * suite, maxn, items, loops);
	  arrclear (rtest -> results, rmspent);
	  if (! * suite)
	    printf ("\n");
	}
      suite ++;
    }
}
