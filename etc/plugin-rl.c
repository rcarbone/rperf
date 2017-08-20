/* System headers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>

/* Project headers */
#include "rlsuite.h"

#define RLSUITE_GROW_HEAD  1
#define RLSUITE_GROW_TAIL  2


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
	case -1:                                                               break;  /* boot */
	case -2:                                                               break;  /* halt */
	case RLSUITE_GROW_HEAD: rlsuite_grow_head (nelems, (relem_t **) envp); break;
	case RLSUITE_GROW_TAIL: rlsuite_grow_tail (nelems, (relem_t **) envp); break;
	}
    }

  default_vals ();

  return 0;
}


int boot (int argc, char * argv [], void * envp [])
{
  return run_this (-1, argc, argv, envp);
}


int halt (int argc, char * argv [], void * envp [])
{
  return run_this (-2, argc, argv, envp);
}


int grow_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RLSUITE_GROW_HEAD, argc, argv, envp);
}


int grow_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RLSUITE_GROW_TAIL, argc, argv, envp);
}
