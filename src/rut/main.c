/* System headers */
#include <stdio.h>
#include <libgen.h>
#include <getopt.h>


/* Project headers */
#include "rut.h"
#include "sargv.h"
#include "support.h"


/* Select/Display/Run Unit Tests */
int main (int argc, char * argv [])
{
  unsigned no = rutargc ();
  rut_t * rl  = rutargv ();
  unsigned mx = rut_maxn (no, rl);
  unsigned i;
  char ** names;
  char ** n;
  rut_t ** handles;
  rut_t ** h;

  printf ("First\n");
  rut_print (rut_first (rl), 2, mx);
  printf ("\n");

  printf ("Last\n");
  rut_print (rut_last (no, rl), 2, mx);
  printf ("\n");

  printf ("At\n");
  for (i = 0; i < no; i ++)
    rut_print (rut_at (no, rl, i), 2, mx);
  printf ("\n");

  printf ("Rand\n");
  for (i = 0; i < no; i ++)
    rut_print (rut_rnd (no, rl), 2, mx);
  printf ("\n");

  printf ("Name\n");
  n = names = rut_names (no, rl);
  while (n && * n)
    rut_print (rut_name (no, rl, * n ++), 2, mx);
  argsclear (names);
  printf ("\n");

  printf ("Run\n");
  for (i = 0; i < no; i ++)
    rut_run (rut_at (no, rl, i), 1000, digits (no), i, mx);
  printf ("\n");

  printf ("Names\n");
  n = names = rut_names (no, rl);
  argsrows (names);
  argsclear (names);
  printf ("\n");

  printf ("Handles\n");
  h = handles = rut_handles (no, rl);
  while (h && * h)
    rut_print (* h ++, 2, mx);
  arrclear (handles, NULL);
  printf ("\n");

  printf ("Handles (shuffle)\n");
  h = handles = rut_shuffle (no, rl);
  while (h && * h)
    rut_print (* h ++, 2, mx);
  arrclear (handles, NULL);
  printf ("\n");

  printf ("Print\n");
  rut_prints (no, rl);

  return 0;
}
