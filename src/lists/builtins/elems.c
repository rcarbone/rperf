/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/* Project headers */
#include "elems.h"
#include "support.h"
#include "safe.h"


relem_t * mkelem (unsigned key)
{
  relem_t * elem = calloc (1, sizeof (* elem));

  elem -> foo = key;
  elem -> bar = strdup (utoa (elem -> foo));
  elem -> me  = elem;

  return elem;
}


void rmelem (relem_t * elem)
{
  if (! elem)
    return;

  safefree (elem -> bar);
  free (elem);
}


/* Initializes the array with the natural numbers from 1 to argc */
relem_t ** mkelems (unsigned argc)
{
  relem_t ** argv = calloc (argc + 1, sizeof (* argv));
  unsigned i;

  for (i = 0; i < argc; i ++)
    argv [i] = mkelem (i + 1);
  argv [i] = NULL;

  return argv;
}


void rmelems (relem_t * argv [])
{
  relem_t ** e = argv;
  while (e && * e)
    rmelem (* e ++);
  safefree (argv);
}
