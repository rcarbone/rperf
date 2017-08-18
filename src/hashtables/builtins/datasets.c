/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/* Project headers */
#include "datasets.h"
#include "support.h"
#include "rht-hashers.h"


robj_t * mkobj (unsigned key, unsigned val, unsigned miss,
		unsigned dense, unsigned sparse, unsigned kbench)
{
  robj_t * obj = calloc (1, sizeof (* obj));

  /* Unsigned Keys */
  obj -> ukey    = key;
  obj -> umiss   = miss + 1;
  obj -> ukey_2  = dense;
  obj -> umiss_2 = obj -> ukey_2 + 1;
  obj -> ukey_3  = sparse;
  obj -> umiss_3 = obj -> ukey_3 + 1;
  obj -> ukbench = kbench;

  obj -> rand_2  = obj -> ukey_2;
  obj -> rand_3  = obj -> ukey_3;

  /* String Keys */
  obj -> skey    = strdup (utoa (obj -> ukey));
  obj -> smiss   = strdup (utoa (obj -> umiss));
  obj -> skey_2  = strdup (utoa (obj -> ukey_2));
  obj -> skey_3  = strdup (utoa (obj -> ukey_3));
  obj -> skbench = strdup (utoa (obj -> ukbench));

  /* Pointer Key */
  obj -> pkey = obj;

  /* Values */
  obj -> uval = val;
  obj -> sval = strdup (utoa (obj -> uval));
  obj -> pval = obj;

  /* Hashes */
  obj -> uhash = rht_java_hash (obj -> ukey);
  obj -> shash = rht_python_hash (obj -> skey);
  obj -> phash = obj -> pkey;

  return obj;
}


void rmobj (robj_t * obj)
{
  if (! obj)
    return;
  if (obj -> skey)
    free (obj -> skey);
  if (obj -> smiss)
    free (obj -> smiss);
  if (obj -> skey_2)
    free (obj -> skey_2);
  if (obj -> skey_3)
    free (obj -> skey_3);
  if (obj -> skbench)
    free (obj -> skbench);
  if (obj -> sval)
    free (obj -> sval);
  free (obj);
}


/* Initializes the array with the natural numbers from 1 to argc */
robj_t ** mkobjs (unsigned argc)
{
  robj_t ** argv = calloc (argc + 1, sizeof (* argv));
  unsigned i;

  srand48 (11);

  for (i = 0; i < argc; i ++)
    argv [i] = mkobj (i + 1, i + 1, argc + i, 0x80000000 + i * 2, 0xffffffff / argc * i,
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


void rmobjs (robj_t * argv [])
{
  robj_t ** o = argv;
  while (o && * o)
    rmobj (* o ++);
  if (argv)
    free (argv);
}
