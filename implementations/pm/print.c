/* System headers */
#include <stdio.h>
#include <libgen.h>


/* Private headers */
#include "plugins.h"


#if defined(ROCCO)
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= *
 *            Builtins             *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
void rp_print_all_builtin_variables (void)
{
  char ** variables = rp_list_builtin_variables ();
  char ** v = variables;

  printf ("List of builtin variables:\n");
  while (v && * v)
    printf ("  %s\n", * v ++);
  argsfree (variables);
}


void rp_print_all_builtin_functions (void)
{
  char ** functions = rp_list_builtin_functions ();
  char ** f = functions;

  printf ("List of builtin functions:\n");
  while (f && * f)
    printf ("  %s\n", * f ++);
  argsfree (functions);
}


void rp_print_all_builtin_flags (void)
{
  char ** flags = rp_list_builtin_flags ();
  char ** f = flags;

  printf ("List of builtin flags:\n");
  while (f && * f)
    printf ("  %s\n", * f ++);
  argsfree (flags);
}


void rp_print_all_builtins (void)
{
  rp_print_all_builtin_variables ();
  printf ("\n");
  rp_print_all_builtin_functions ();
  printf ("\n");
  rp_print_all_builtin_flags ();
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= *
 *            Variables            *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
void rp_print_plugin_info_general (PluginEntryPoint * p)
{
  printf ("    General\n");
  printf ("     name         : %s\n", p -> name                                   ? p -> name            : NONE);
  printf ("     author       : %s\n", p -> author                                 ? p -> author          : NONE);
  printf ("     version      : %s\n", p -> version                                ? p -> version         : NONE);
  printf ("     description  : %s\n", p -> descr                                  ? p -> descr           : NONE);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= *
 *            Functions            *
 * =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
void rp_print_plugin_info_functions (PluginEntryPoint * p)
{
  printf ("    Functions\n");
  printf ("      Help        : %s\n",    p -> helpFctn                            ? DEFINED              : UNDEFINED);
  printf ("      Init        : %s\n",    p -> initFctn                            ? DEFINED              : UNDEFINED);
  printf ("      Cleanup     : %s\n",    p -> termFctn                            ? DEFINED              : UNDEFINED);
}

#endif /* ROCCO */

