/* Project headers */
#include "vargs.h"
#include "rtest.h"

#define SEP  '|'
#define MAXN 7


static void runit_print_one (rtest_t * runit, unsigned n)
{
  if (runit)
    printf ("%3d%c %-*.*s %c%3d %c %s\n", n, SEP, MAXN, MAXN, runit -> name, SEP, runit -> id, SEP, runit -> description);
}


static void runit_print_header (void)
{
  printf (" # %c %-*.*s %c %s %c %s\n", SEP, MAXN, MAXN, "Name", SEP, "Id", SEP, "Description");
  printf ("--- %s %c %s %c %s\n", "--------", SEP, "--", SEP, "----------------------");
}


#if defined(ROCCO)
void runit_print_first (void)
{
  printf ("First Unit Test\n");
  runit_print_one (runit_first (), 1);
}


void runit_print_last (void)
{
  printf ("Last Unit Test\n");
  runit_print_one (runit_last (), runit_no ());
}


/* Foreach loop version */
void runit_print_foreach (void)
{
  rtest_t * first = runit_next ();
  rtest_t * next;
  int n = 0;

  if (! first)
    return;

  runit_print_no ();
  runit_print_one (first, ++ n);

  while ((next = runit_next ()) != first)
    runit_print_one (next, ++ n);
}


void runit_all_print (void)
{
  unsigned n = 0;
  rtest_t ** all = runit_all ();
  rtest_t ** a = all;

  runit_print_no ();
  while (a && * a)
    runit_print_one (* a ++, ++ n);
  arrclear (all, NULL);
}


void runit_all_n_print (void)
{
  unsigned n = 0;
  rtest_t ** all = runit_all_n (69);
  rtest_t ** a = all;

  runit_print_no ();
  while (a && * a)
    runit_print_one (* a ++, ++ n);
  arrclear (all, NULL);
}


void runit_all_rnd_print (void)
{
  unsigned n = 0;
  rtest_t ** all = runit_all_rnd ();
  rtest_t ** a = all;

  runit_print_no ();
  while (a && * a)
    runit_print_one (* a ++, ++ n);
  arrclear (all, NULL);
}

#endif /* ROCCO */


/* -=-=-=-=-=-=-= API -=-=-=-=-=-=-= */


void runit_print_no (void)
{
  printf ("Built-in Units Tests : %u\n", runit_no ());
}


/* Print the Unit Tests included in argv[] */
void runit_print (rtest_t * argv [])
{
  unsigned i = 0;

  runit_print_header ();
  while (argv && * argv)
    runit_print_one (* argv ++, ++ i);
}


/* Print the all the builtin Unit Tests */
void runit_print_all (void)
{
  unsigned i;

  runit_print_header ();
  for (i = 0; i <= runit_no (); i ++)
    runit_print_one (runit_find_at (i), i + 1);
}


void rsuite_print_no (void)
{
  printf ("Built-in Units Tests : %u\n", rsuite_no ());
}


/* Print the Test Suite included in argv[] */
void rsuite_print (rtest_t * argv [])
{
  unsigned i = 0;

  runit_print_header ();
  while (argv && * argv)
    runit_print_one (* argv ++, ++ i);
}


/* Print the all the builtin Test Suite */
void rsuite_print_all (void)
{
  unsigned i;

  runit_print_header ();
  for (i = 0; i <= rsuite_no (); i ++)
    runit_print_one (rsuite_find_at (i), i + 1);
}


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
