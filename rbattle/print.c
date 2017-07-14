/* Project headers */
#include "rbattle.h"


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

#if defined(ROCCO)
void hall_of_fame (char * name, result_t ** results, unsigned all, unsigned maxn,
		   unsigned initials, unsigned repeat, unsigned more)
{
  unsigned t = 0;
  unsigned j = 0;
  unsigned i;

  if (initials % 10)
    printf ("Test [%s]: %u initial items (+%u)\n", name, initials, more);
  else
    printf ("Test [%s]: 1e%u initial items (+%u)\n", name, digits (initials) - 1, more);

  printf ("      %-*.*s    items     -  time spent           avg         min         max      ms\n",
	  maxn, maxn, "Name");

  for (i = 0; i < all; i ++)
    if (results [t] [i] . sw && results [t] [i] . ciao)
      {
	printf ("  %2d: %-*.*s  ", ++ j, maxn, maxn, results [t] [i] . sw -> name);
	old_show_info (results [t] [i] . sw, results [t] [i] . ciao, maxn);
	t ++;
      }
  printf ("\n");
}
#endif /* ROCCO */
