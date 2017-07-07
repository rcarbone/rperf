/* Public header */
#include "rbattle.h"


/* Display information for a single test run */
void old_show_info (sw_t * sw, test_t * test, unsigned maxn)
{
  if (test)
    printf ("- [%-8u] - %12.12s -    %8.3f  / %8.3f  / %8.3f\n",
	    test -> items,
	    ns2a (test -> elapsed),
	    test -> avg / 1e6,
	    test -> min / 1e6,
	    test -> max / 1e6);
}


void hall_of_fame (unsigned t, result_t ** results, unsigned all, unsigned maxn,
		   unsigned initials, unsigned repeat, unsigned more)
{
  unsigned j = 0;
  unsigned i;

  if (initials % 10)
    printf ("Test [%s]: %u initial items (+%u)\n", tname (t), initials, more);
  else
    printf ("Test [%s]: 1e%u initial items (+%u)\n", tname (t), digits (initials) - 1, more);

  printf ("      %-*.*s    items     -  time spent           avg         min         max      ms\n",
	  maxn, maxn, "Name");

  for (i = 0; i < all; i ++)
    if (results [t] [i] . sw && results [t] [i] . test)
      {
	printf ("  %2d: %-*.*s  ", ++ j, maxn, maxn, results [t] [i] . sw -> name);
	old_show_info (results [t] [i] . sw, results [t] [i] . test, maxn);
      }
  printf ("\n");
}
