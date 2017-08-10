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


static int run_this (int which, int argc, char * argv [], void * envp [])
{
  parse_options (argc, argv, envp);

  if (verbose)
    printf ("%s/%s: Hello World! My name is %s", library, module, argv [0]);

  if (nobjs)
    {
      switch (which)
	{
	case -1:                                                               break;  /* boot */
	case -2:                                                               break;  /* halt */
	case RSUITE_GROW_SEQ:    rsuite_grow_seq (nobjs, (robj_t **) envp);    break;
	case RSUITE_GROW_RND:    rsuite_grow_rnd (nobjs, (robj_t **) envp);    break;
	case RSUITE_HIT_SEQ:     rsuite_hit_seq (nobjs, (robj_t **) envp);     break;
	case RSUITE_HIT_RND:     rsuite_hit_rnd (nobjs, (robj_t **) envp);     break;
	case RSUITE_MISS_SEQ:    rsuite_miss_seq (nobjs, (robj_t **) envp);    break;
	case RSUITE_MISS_RND:    rsuite_miss_rnd (nobjs, (robj_t **) envp);    break;
	case RSUITE_DELETE_SEQ:  rsuite_delete_seq (nobjs, (robj_t **) envp);  break;
	case RSUITE_DELETE_RND:  rsuite_delete_rnd (nobjs, (robj_t **) envp);  break;
	case RSUITE_REPLACE_SEQ: rsuite_replace_seq (nobjs, (robj_t **) envp); break;
	case RSUITE_REPLACE_RND: rsuite_replace_rnd (nobjs, (robj_t **) envp); break;
	case RSUITE_KBENCH:      rsuite_kbench (nobjs, (robj_t **) envp);      break;
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
  return run_this (RSUITE_GROW_SEQ, argc, argv, envp);
}


int grow_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_GROW_RND, argc, argv, envp);
}


int hit_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_HIT_SEQ, argc, argv, envp);
}


int hit_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_HIT_RND, argc, argv, envp);
}


int miss_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_MISS_SEQ, argc, argv, envp);
}


int miss_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_MISS_RND, argc, argv, envp);
}


int delete_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_DELETE_SEQ, argc, argv, envp);
}


int delete_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_DELETE_RND, argc, argv, envp);
}


int replace_seq (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_REPLACE_SEQ, argc, argv, envp);
}


int replace_rnd (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_REPLACE_RND, argc, argv, envp);
}


int kbench (int argc, char * argv [], void * envp [])
{
  return run_this (RSUITE_KBENCH, argc, argv, envp);
}
