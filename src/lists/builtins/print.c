/* Project headers */
#include "vargv.h"
#include "rltest.h"

#define SEP  '|'


static void rlunit_print_header (unsigned maxn)
{
  printf (" # %c %-*.*s %c %s %c %s\n", SEP, maxn, maxn, "Name", SEP, "Id", SEP, "Description");
  printf ("--- %s %c %s %c %s\n", "--------", SEP, "--", SEP, "----------------------");
}


static void rlunit_print_one (rltest_t * rlunit, unsigned n, unsigned maxn)
{
  if (rlunit)
    printf ("%3d%c %-*.*s %c%3d %c %s\n", n, SEP, maxn, maxn, rlunit -> name, SEP, rlunit -> id, SEP, rlunit -> description);
}


#if defined(ROCCO)
void rlunit_print_first (void)
{
  printf ("First Unit Test\n");
  rlunit_print_one (rlunit_first (), 1);
}


void rlunit_print_last (void)
{
  printf ("Last Unit Test\n");
  rlunit_print_one (rlunit_last (), rlunit_no ());
}


/* Foreach loop version */
void rlunit_print_foreach (void)
{
  rltest_t * first = rlunit_next ();
  rltest_t * next;
  int n = 0;

  if (! first)
    return;

  rlunit_print_no ();
  rlunit_print_one (first, ++ n);

  while ((next = rlunit_next ()) != first)
    rlunit_print_one (next, ++ n);
}


void rlunit_all_print (void)
{
  unsigned n = 0;
  rltest_t ** all = rlunit_all ();
  rltest_t ** a = all;

  rlunit_print_no ();
  while (a && * a)
    rlunit_print_one (* a ++, ++ n);
  arrclear (all, NULL);
}


void rlunit_all_n_print (void)
{
  unsigned n = 0;
  rltest_t ** all = rlunit_all_n (69);
  rltest_t ** a = all;

  rlunit_print_no ();
  while (a && * a)
    rlunit_print_one (* a ++, ++ n);
  arrclear (all, NULL);
}


void rlunit_all_rnd_print (void)
{
  unsigned n = 0;
  rltest_t ** all = rlunit_all_rnd ();
  rltest_t ** a = all;

  rlunit_print_no ();
  while (a && * a)
    rlunit_print_one (* a ++, ++ n);
  arrclear (all, NULL);
}

#endif /* ROCCO */


/* -=-=-=-=-=-=-= API -=-=-=-=-=-=-= */


void rlunit_print_no (void)
{
  printf ("Built-in Units Tests : %u\n", rlunit_no ());
}


/* Print the Unit Tests included in argv[] */
void rlunit_print (rltest_t * argv [])
{
  unsigned maxn = rlunit_maxn (argv);
  unsigned i = 0;

  rlunit_print_header (maxn);
  while (argv && * argv)
    rlunit_print_one (* argv ++, ++ i, maxn);
}


/* Print the all the builtin Unit Tests */
void rlunit_print_all (void)
{
  unsigned maxn = rlunit_all_maxn ();
  unsigned i;

  rlunit_print_header (maxn);
  for (i = 0; i <= rlunit_no (); i ++)
    rlunit_print_one (rlunit_find_at (i), i + 1, maxn);
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
