/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/* Project headers */
#include "robject.h"
#include "safe.h"
#include "support.h"


robj_t * mkobj (unsigned x)
{
  robj_t * obj = (robj_t *) calloc (1, sizeof (* obj));

  obj -> key = strdup (utoa (x));
  obj -> val = x;
  obj -> foo = obj;
  obj -> len = strlen (obj -> key);

  return obj;
}


void rmobj (robj_t * obj)
{
  if (! obj)
    return;

  safefree (obj -> key);
  free (obj);
}


int cmpobj (const void * n1, const void * n2)
{
  return strcmp (((robj_t *) n1) -> key, ((robj_t *) n2) -> key);
}


/* Initializes the array with the natural numbers from 1 to argc */
robj_t ** mkobjs (unsigned argc)
{
  robj_t ** argv = (robj_t **) calloc (argc + 1, sizeof (* argv));
  unsigned i;

  for (i = 0; i < argc; i ++)
    argv [i] = mkobj (i + 1);
  argv [i] = NULL;

  return argv;
}


void rmobjs (robj_t * argv [])
{
  robj_t ** e = argv;
  while (e && * e)
    rmobj (* e ++);
  safefree (argv);
}


/* Initializes the array with the natural numbers from argc + 1 to 2 * argc */
robj_t ** mkmiss (unsigned argc)
{
  robj_t ** argv = (robj_t **) calloc (argc + 1, sizeof (* argv));
  unsigned i;

  for (i = 0; i < argc; i ++)
    argv [i] = mkobj (argc + i + 1);
  argv [i] = NULL;

  return argv;
}
