/* System headers */
#include <stdbool.h>
#include <ctype.h>


static bool isnumber (char d)
{
  switch (d)
    {
    case '0': case '1': case '2': case '3': case '4':
    case '5': case '6': case '7': case '8': case '9':
      return true;
    default:
      return false;
    }
}


/* Is the string passed by 's' numeric? */
bool isnumeric (char * s)
{
  if (! s || ! * s)
    return false;

  while (* s)
    if (! isnumber ((int) * s ++))
      return false;

  return true;
}
