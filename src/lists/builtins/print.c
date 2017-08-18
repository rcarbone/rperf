/* System headers */
#include <string.h>

/* Project headers */
#include "vargv.h"
#include "rltest.h"


void print_dots (char * name, char * label, unsigned n, unsigned seq, unsigned maxn)
{
  unsigned dots = name ? strlen (name) : 0;
  unsigned i;
  char fmt [1024];
  char str [1014];

  sprintf (fmt, "[%%%uu] %s %%s ", n, label);
  sprintf (str, fmt, seq, name);

  if (dots < maxn)
    dots = maxn - dots + 3;
  else
    dots = 3;

  printf ("%s", str);

  for (i = 0; i < dots; i ++)
    printf (".");
  printf (" ");
}
