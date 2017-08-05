/* System headers */
#include <stdlib.h>


/* Project headers */
#include "support.h"
#include "rwall.h"


/*
 * http://stackoverflow.com/questions/6127503/shuffle-array-in-c
 *
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
      unsigned j;
      unsigned t;
      for (i = 0; i < n - 1; i ++)
        {
          j = i + rand () / (RAND_MAX / (n - i) + 1);
          t = array [j];
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


/* Convert an integer to a string (more efficient than sprintf) */
#define BASE 10
void itoa (int c, char * ret)
{
  static const char * exa = "0123456789abcdef";

  if (c == 0)
    ret [0] = '0',
      ret [1] = '\0';
  else
    {
      int l, x, y;
      char buf [16];

      for (l = 0, x = c < 0 ? -c : c; x > 0; x /= BASE)
        buf [l ++] = exa [x % BASE];

      if (c < 0)
        buf [l ++] = '-';

      for (x = l - 1, y = 0; x >= 0; x --)
        ret [y ++] = buf [x];

      ret [y] = '\0';
    }
}


/* Evaluate the number of digits in n */
unsigned digits (unsigned n)
{
  return n < 10 ? 1 : 1 + digits (n / 10);
}
