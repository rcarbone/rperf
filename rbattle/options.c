/* System headers */
#include <stdio.h>
#include <string.h>
#include <getopt.h>
#include <ctype.h>


#define MAX(a,b)  (a > b ? a : b)


/* Evaluate the max length of option names */
unsigned optmax (struct option * options)
{
  unsigned max = 0;

  /* Iterate over all options */
  while ((* options) . name)
    {
      max = MAX (max, strlen ((* options) . name));
      options ++;
    }
  return max;
}


/* Evaluate the string containing the legitimate short option characters */
char * optlegitimate (struct option * options)
{
  static char buffer [1024];

  buffer [0] = 0x0;

  /* Iterate over all options */
  while ((* options) . name)
    {
      if (isascii ((* options) . val))
	{
	  switch ((* options) . has_arg)
	    {
	    case optional_argument: sprintf (buffer + strlen (buffer), "%c::", (* options) . val); break;
	    case required_argument: sprintf (buffer + strlen (buffer), "%c:",  (* options) . val); break;
	    case no_argument:	    sprintf (buffer + strlen (buffer), "%c",   (* options) . val); break;
	    }
	}
      options ++;
    }
  return buffer;
}


/* Lookup for a name in the table of long options */
char * optname (struct option * options, unsigned n, unsigned val)
{
  static char buffer [1024];

  /* Iterate over all options */
  while ((* options) . name)
    if ((* options) . val == val)
      break;
    else
      options ++;

  n += 9;  /* <val> */
  if ((* options) . name && (* options) . has_arg == required_argument)
    {
      char required [1024];
      sprintf (required, "%s <val>", (* options) . name);
      sprintf (buffer, "%-*.*s", n, n, required);
    }
  else
    sprintf (buffer, "%-*.*s", n, n, (* options) . name ? (* options) . name : "???");
  return buffer;
}


/* Display a single item of the usage */
void usage_item (struct option * options, unsigned n, unsigned val, char * description)
{
  if (! description)
    description = "";

  if (isascii (val))
    printf ("   -%c | --%s  %s\n", val, optname (options, n, val), description);
  else
    printf ("        --%s  %s\n", optname (options, n, val), description);
}
