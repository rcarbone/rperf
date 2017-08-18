/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/* Project headers */
#include "elems.h"
#include "support.h"
#include "safe.h"


relem_t * mkelem (unsigned key, unsigned val, unsigned miss)
{
  relem_t * elem = calloc (1, sizeof (* elem));

  /* Unsigned Keys */
  elem -> ukey  = key;
  elem -> umiss = miss + 1;

  /* String Keys */
  elem -> skey  = strdup (utoa (elem -> ukey));
  elem -> smiss = strdup (utoa (elem -> umiss));

  /* Pointer Key */
  elem -> pkey = elem;

  /* Values */
  elem -> uval = val;
  elem -> sval = strdup (utoa (elem -> uval));
  elem -> pval = elem;

  return elem;
}


void rmelem (relem_t * elem)
{
  if (! elem)
    return;

  safefree (elem -> skey);
  safefree (elem -> smiss);
  safefree (elem -> sval);
  free (elem);
}


/* Initializes the array with the natural numbers from 1 to argc */
relem_t ** mkelems (unsigned argc)
{
  relem_t ** argv = calloc (argc + 1, sizeof (* argv));
  unsigned i;

  for (i = 0; i < argc; i ++)
    argv [i] = mkelem (i + 1, i + 1, argc + i);
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
