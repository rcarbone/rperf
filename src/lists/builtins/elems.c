/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/* Project headers */
#include "elems.h"
#include "safe.h"


/* Unsigned to Ascii */
static char * utoa (unsigned n)
{
  /* Limits for static buffers in utoa() */
#define ITEMS 10      /* rows */
#define SIZE  16      /* cols */

  static char text [ITEMS] [SIZE];
  static unsigned i = 0;

  char * s = text [i ++ % ITEMS];
  sprintf (s, "%u", n);
  return s;
}


relem_t * mkelem (unsigned key, unsigned val, unsigned miss,
		  unsigned dense, unsigned sparse, unsigned kbench)
{
  relem_t * elem = calloc (1, sizeof (* elem));

  /* Unsigned Keys */
  elem -> ukey    = key;
  elem -> umiss   = miss + 1;
  elem -> ukey_2  = dense;
  elem -> umiss_2 = elem -> ukey_2 + 1;
  elem -> ukey_3  = sparse;
  elem -> umiss_3 = elem -> ukey_3 + 1;
  elem -> ukbench = kbench;

  elem -> rand_2  = elem -> ukey_2;
  elem -> rand_3  = elem -> ukey_3;

  /* String Keys */
  elem -> skey    = strdup (utoa (elem -> ukey));
  elem -> smiss   = strdup (utoa (elem -> umiss));
  elem -> skey_2  = strdup (utoa (elem -> ukey_2));
  elem -> skey_3  = strdup (utoa (elem -> ukey_3));
  elem -> skbench = strdup (utoa (elem -> ukbench));

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
  safefree (elem -> skey_2);
  safefree (elem -> skey_3);
  safefree (elem -> skbench);
  safefree (elem -> sval);
  free (elem);
}


/* Initializes the array with the natural numbers from 1 to argc */
relem_t ** mkelems (unsigned argc)
{
  relem_t ** argv = calloc (argc + 1, sizeof (* argv));
  unsigned i;

  srand48 (11);

  for (i = 0; i < argc; i ++)
    argv [i] = mkelem (i + 1, i + 1, argc + i, 0x80000000 + i * 2, 0xffffffff / argc * i,
		      (argc * drand48 () / 4) * 271828183u);
  argv [i] = NULL;

#if defined(ROCCO)
  /*
   * randomize using the Fisher-Yates shuffle
   * http://en.wikipedia.org/wiki/Fisher-Yates_shuffle
   */
  for (i = argc - 1; i >= 0; -- i)
    {
      unsigned j = rnd (i + 1);
      unsigned t = argv [i] -> rand_2;

      argv [i] -> rand_2 = argv [j] -> rand_2;
      argv [j] -> rand_2 = t;

      j = rnd (i + 1);
      t = argv [i] -> rand_3;

      argv [i] -> rand_3 = argv [j] -> rand_3;
      argv [j] -> rand_3 = t;
    }
#endif /* ROCCO */

  return argv;
}


void rmelems (relem_t * argv [])
{
  relem_t ** e = argv;
  while (e && * e)
    rmelem (* e ++);
  safefree (argv);
}
