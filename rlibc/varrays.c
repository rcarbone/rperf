/*
 * Utilities to handle dynamic arrays of pointers to void objects
 */


/* Public header */
#include "varrays.h"


/* Return the # of items in an array */
unsigned valen (void * argv [])
{
  unsigned argc = 0; while (argv && * argv ++) argc ++; return argc;
}


/* Add an item to an array */
void ** vamore (void * argv [], void * item)
{
  if (item)
    {
      unsigned argc = valen (argv);
      argv = realloc (argv, (1 + argc + 1) * sizeof (void **));
      if (! argv)
	return NULL;
      argv [argc ++] = item;
      argv [argc]    = NULL;         /* NULL terminate the array */
    }
  return argv;
}


/* Remove an item from an array */
void ** valess (void * argv [], void * item, varm_f rm)
{
  int i;
  int j;
  int argc = valen (argv);

  if ((i = vahas (argv, item)) != -1)
    {
      if (rm)
	rm (argv [i]);                   /* free the item */
      for (j = i; j < argc - 1; j ++)    /* move pointers back one position */
        argv [j] = argv [j + 1];

      argv [j] = NULL;                   /* NULL terminate the array */

      if (argc > 1)
        argv = realloc (argv, argc * sizeof (void *));
      else
        free (argv);
    }

  return argc > 1 ? argv : NULL;
}


/* Lookup by reference for an item into an array */
int vahas (void * argv [], void * item)
{
  int found = 0;

  while (argv && * argv)
    if (* argv ++ == item)
      return found;
    else
      found ++;

  return -1;
}


/* Cleanup an array */
void ** vaclear (void * argv [], varm_f rm)
{
  void ** r = argv;
  while (r && * r)
    {
      if (rm)
	rm (* r);
      r ++;
    }
  if (argv)
    free (argv);
  return NULL;
}


/* Duplicate an array */
void ** vadup (void * argv [])
{
  void ** dup = NULL;
  if (argv)
    while (* argv)
      dup = vamore (dup, * argv ++);

  return dup;
}


/* Reverse an array */
void ** varev (void * argv [])
{
  void ** rev = NULL;
  unsigned argc = valen (argv);
  if (argv)
    while (argc)
      rev = vamore (rev, argv [-- argc]);
  return rev;
}


/* Concatenate array 'b' to 'a' */
void ** vacat (void * a [], void * b [])
{
  while (b && * b)
    a = vamore (a, * b ++);
  return a;
}


/* Sort an array */
void ** vasort (void * argv [], vasort_f * cmp)
{
  if (! argv || ! cmp)
    return argv;

  qsort (argv, valen (argv), sizeof (void *), cmp);

  return argv;
}
