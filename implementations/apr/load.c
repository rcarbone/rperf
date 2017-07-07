/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* Project headers */
#include "plugins.h"


/* Load all plugins passed on the command line and show their symbols */
int main (int argc, char * argv [])
{
  bool show_names     = false;
  bool show_variables = false;
  bool show_functions = false;
  unsigned i = 0;
  int option;

#define OPTSTRING "nvf"
  while ((option = getopt (argc, argv, OPTSTRING)) != -1)
    {
      switch (option)
        {
	default: return 0;

	case 'n': show_names = true;     argc --; argv ++; break;
	case 'v': show_variables = true; argc --; argv ++; break;
	case 'f': show_functions = true; argc --; argv ++; break;
	}
    }

  /* Process each argument passed on the command line */
  while (-- argc)
    {
      char * path = argv [++ i];
      int error;
      char * buffer;
      plugin_t * p;

      p = mkplugin (path, & error, & buffer);
      if (p)
	{
	  char ** n;
	  symbol_t ** s;

	  printf ("plugin %s: loaded!\n", path);

	  if (show_names)
	    {
	      n = p -> declared;
	      if (n)
		printf ("  defined names\n");
	      while (n && * n)
		printf ("    %s\n", * n ++);
	    }

	  if (show_variables)
	    {
	      s = p -> vars;
	      if (s)
		printf ("  defined variables\n");
	      while (s && * s)
		{
		  printf ("    [var] %s - %s\n", (* s) -> name, variable ((* s) -> name, p -> vars));
		  s ++;
		}
	    }

	  if (show_functions)
	    {
	      s = p -> func;
	      if (s)
		printf ("  defined functions\n");
	      while (s && * s)
		{
		  call_t * fun = function ((* s) -> name, p -> func);
		  char * argv [2] = { (* s) -> name, NULL };

		  printf ("    [fun] running %s ... ", (* s) -> name);
		  if (! fun (2, argv, NULL))
		    printf ("Ok\n");
		  else
		    printf ("No\n");
		  s ++;
		}
	    }
	  rmplugin (p);
	}
      else
	{
	  printf ("Cannot load %s: [%s]\n", path, buffer);
	  free (buffer);
	}
    }

  if (! i)
    printf ("Missing filename\n");

  return 0;
}
