/* System headers */
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <getopt.h>

/* Project headers */
#include "rht-test.h"
#include "rht-suite.h"

/* The local identifiers for the Test Scenarios to run */
typedef enum
{
  RHTSUITE_BOOT        = 1,
  RHTSUITE_HALT,
  RHTSUITE_GROW_SEQ,
  RHTSUITE_GROW_RND,
  RHTSUITE_HIT_SEQ,
  RHTSUITE_HIT_RND,
  RHTSUITE_MISS_SEQ,
  RHTSUITE_MISS_RND,
  RHTSUITE_DELETE_SEQ,
  RHTSUITE_DELETE_RND,
  RHTSUITE_REPLACE_SEQ,
  RHTSUITE_REPLACE_RND,
  RHTSUITE_KBENCH

} rlsuite_id_t;


static unsigned nobjs = 0;
static bool verbose   = false;


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


static int run_this (int which, int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  if (nobjs)
    {
      switch (which)
	{
	case RHTSUITE_BOOT:                                            break;
	case RHTSUITE_HALT:                                            break;
	case RHTSUITE_GROW_SEQ:    rhtsuite_grow_seq (nobjs, envp);    break;
	case RHTSUITE_GROW_RND:    rhtsuite_grow_rnd (nobjs, envp);    break;
	case RHTSUITE_HIT_SEQ:     rhtsuite_hit_seq (nobjs, envp);     break;
	case RHTSUITE_HIT_RND:     rhtsuite_hit_rnd (nobjs, envp);     break;
	case RHTSUITE_MISS_SEQ:    rhtsuite_miss_seq (nobjs, envp);    break;
	case RHTSUITE_MISS_RND:    rhtsuite_miss_rnd (nobjs, envp);    break;
	case RHTSUITE_DELETE_SEQ:  rhtsuite_delete_seq (nobjs, envp);  break;
	case RHTSUITE_DELETE_RND:  rhtsuite_delete_rnd (nobjs, envp);  break;
	case RHTSUITE_REPLACE_SEQ: rhtsuite_replace_seq (nobjs, envp); break;
	case RHTSUITE_REPLACE_RND: rhtsuite_replace_rnd (nobjs, envp); break;
	case RHTSUITE_KBENCH:      rhtsuite_kbench (nobjs, envp);      break;
	}
    }

  default_vals ();

  return 0;
}


int boot (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_BOOT, argc, argv, envp);
}


int halt (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_HALT, argc, argv, envp);
}


int grow_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_GROW_SEQ, argc, argv, envp);
}


int grow_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_GROW_RND, argc, argv, envp);
}


int hit_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_HIT_SEQ, argc, argv, envp);
}


int hit_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_HIT_RND, argc, argv, envp);
}


int miss_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_MISS_SEQ, argc, argv, envp);
}


int miss_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_MISS_RND, argc, argv, envp);
}


int delete_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_DELETE_SEQ, argc, argv, envp);
}


int delete_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_DELETE_RND, argc, argv, envp);
}


int replace_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_REPLACE_SEQ, argc, argv, envp);
}


int replace_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_REPLACE_RND, argc, argv, envp);
}


int kbench (int argc, char * argv [], void * envp [])
{
  return run_this (RHTSUITE_KBENCH, argc, argv, envp);
}
