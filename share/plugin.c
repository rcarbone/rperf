/* System headers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>

/* Project headers */
#include "rtest.h"
#include "rsuite.h"

static unsigned nobjs = 0;
static bool verbose = false;


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
	default:                         break;
	case 'v': verbose = true;        break;
	case 'n': nobjs = atoi (optarg); break;
	}
    }
}


static void default_vals (void)
{
  nobjs = 0;
  verbose = false;
}


int boot (int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  default_vals ();

  return 0;
}


int halt (int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Bye bye cruel world!", library, module);

  default_vals ();

  return 0;
}


int grow (int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  if (nobjs)
    rsuite_grow (nobjs, (robj_t **) envp);

  default_vals ();

  return 0;
}


int hit (int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  if (nobjs)
    rsuite_hit (nobjs, (robj_t **) envp);

  default_vals ();

  return 0;
}


int miss (int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  if (nobjs)
    rsuite_miss (nobjs, (robj_t **) envp);

  default_vals ();

  return 0;
}


int delete (int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  if (nobjs)
    rsuite_delete (nobjs, (robj_t **) envp);

  default_vals ();

  return 0;
}


int replace (int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  if (nobjs)
    rsuite_replace (nobjs, (robj_t **) envp);

  default_vals ();

  return 0;
}


int kbench (int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  if (nobjs)
    rsuite_kbench (nobjs, (robj_t **) envp);

  default_vals ();

  return 0;
}
