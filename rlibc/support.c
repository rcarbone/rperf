/* Public header */
#include "support.h"


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


unsigned digits (unsigned n)
{
  return n < 10 ? 1 : 1 + digits (n / 10);
}


