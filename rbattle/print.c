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

