/* System headers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>

/* Project headers */
#include "rsuite.h"


/* The local identifiers for the Test Scenarios to run */
typedef enum
{
  RSUITE_BOOT       = 1,
  RSUITE_HALT,
  RSUITE_GROW,
  RSUITE_ITERATE,
  RSUITE_POP,

} rsuite_id_t;


static unsigned nelems = 0;
static bool verbose    = false;


/* All the functions implemented share the same options */
#define OPTSTRING "vn:"
static void parse_options (int argc, char * argv [], void * envp [])
{
  int option;

  optind = 0;
  optarg = NULL;

  while ((option = getopt (argc, argv, OPTSTRING)) != -1)
    {
      switch (option)
        {
	default:                          break;
	case 'v': verbose = true;         break;
	case 'n': nelems = atoi (optarg); break;
	}
    }
}


static void default_vals (void)
{
  nelems = 0;
  verbose = false;
}


static int run_this (int which, int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  if (nelems)
    {
      switch (which)
	{
	case RSUITE_BOOT:                                   break;
	case RSUITE_HALT:                                   break;
	case RSUITE_GROW:    rsuite_grow (nelems, envp);    break;
	case RSUITE_ITERATE: rsuite_iterate (nelems, envp); break;
	case RSUITE_POP:     rsuite_pop (nelems, envp);     break;
	}
    }

  default_vals ();

  return 0;
}


int boot (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_BOOT, argc, argv, envp);
}


int halt (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_HALT, argc, argv, envp);
}


int grow (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_GROW, argc, argv, envp);
}


int iterate (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_ITERATE, argc, argv, envp);
}


int pop (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_POP, argc, argv, envp);
}
