/* System headers */
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>


/* Public header */
#include "rwall.h"
#include "rspeed.h"


/*
 * http://stackoverflow.com/questions/6127503/shuffle-array-in-c
 */
#include <stdlib.h>

/*
 * Arrange the N elements of ARRAY in random order.
 * Only effective if N is much smaller than RAND_MAX;
 * if this may not be the case, use a better random
 * number generator.
 */
static void shuffle (unsigned n, unsigned * array)
{
  srand (nswall ());

  if (n > 1)
    {
      unsigned i;
      for (i = 0; i < n - 1; i ++)
        {
          unsigned j = i + rand () / (RAND_MAX / (n - i) + 1);
          int t = array [j];
          array [j] = array [i];
          array [i] = t;
        }
    }
}


/* Evaluate a random order to run implementation */
unsigned * rndorder (unsigned n)
{
  unsigned * array = calloc (n, sizeof (unsigned));
  unsigned i;
  for (i = 0; i < n; i ++)
    array [i] = i;
  shuffle (n, array);
  return array;
}
