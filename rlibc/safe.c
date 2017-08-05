/* Project headers */
#include "safe.h"


void * safefree (void * a)
{
  if (a) free (a);
  return NULL;
}


void * safedup (void * a, void * b)
{
  safefree (b);
  return a ? strdup (a) : a;
}


FILE * safeopen (char * f)
{
  return ! strcmp (f, "*") ? NULL : ! strcmp (f, "-") ? stdout : fopen (f, "a+");
}


FILE * safeclose (FILE * fd)
{
  if (fd && fd != stdout)
    fclose (fd);
  return NULL;
}
