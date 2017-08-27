/* System headers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>

/* Project headers */
#include "rlsuite.h"


/* The local identifiers for the Test Scenarios to run */
typedef enum
{
  RLSUITE_BOOT       = 1,
  RLSUITE_HALT,
  RLSUITE_GROW_HEAD,
  RLSUITE_GROW_TAIL,
  RLSUITE_ITERATE,
  RLSUITE_POP_HEAD,
  RLSUITE_POP_TAIL

} rlsuite_id_t;


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
	case RLSUITE_BOOT:                                        break;
	case RLSUITE_HALT:                                        break;
	case RLSUITE_GROW_HEAD: rlsuite_grow_head (nelems, envp); break;
	case RLSUITE_GROW_TAIL: rlsuite_grow_tail (nelems, envp); break;
	case RLSUITE_ITERATE:   rlsuite_iterate (nelems, envp);   break;
	case RLSUITE_POP_HEAD:  rlsuite_pop_head (nelems, envp);  break;
	case RLSUITE_POP_TAIL:  rlsuite_pop_tail (nelems, envp);  break;
	}
    }

  default_vals ();

  return 0;
}


int boot (int argc, char * argv [], void * envp [])
{
  return run_this (RLSUITE_BOOT, argc, argv, envp);
}


int halt (int argc, char * argv [], void * envp [])
{
  return run_this (RLSUITE_HALT, argc, argv, envp);
}


int grow_head (int argc, char * argv [], void * envp [])
{
  return run_this (RLSUITE_GROW_HEAD, argc, argv, envp);
}


int grow_tail (int argc, char * argv [], void * envp [])
{
  return run_this (RLSUITE_GROW_TAIL, argc, argv, envp);
}


int iterate (int argc, char * argv [], void * envp [])
{
  return run_this (RLSUITE_ITERATE, argc, argv, envp);
}


int pop_head (int argc, char * argv [], void * envp [])
{
  return run_this (RLSUITE_POP_HEAD, argc, argv, envp);
}


int pop_tail (int argc, char * argv [], void * envp [])
{
  return run_this (RLSUITE_POP_TAIL, argc, argv, envp);
}
