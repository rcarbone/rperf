/* System headers */
#include <stdio.h>
#include <limits.h>
#include <sys/utsname.h>


/* librht - an abstract C library over real hash tables */
typedef struct rht rht_t;
#include "rht.h"


/* The name of the game with its version number (major), release number (minor) and patchlevel (lower) */
static char __pkgname__     [] = "librht";
static char __version__     [] = "version "RHT_VERSION;
static char __title__       [] = "hash tables without a hash table";
static char __released__    [] = "Jul 2017";
static char __description__ [] = "librht - a C library for virtualization of hash tables";
static char __author__      [] = "Rocco Carbone <rocco@tecsiel.it>";


/* The string version */
static const char rht_version_string [] = "librht version "RHT_VERSION;


const char * rht_lib_version (void)
{
  return rht_version_string;
}


/* Print software information */
void rht_print_version (void)
{
  struct utsname name;

  uname (& name);
  printf ("\n");
  printf ("%s %s %s of %s\n", __pkgname__, __version__, __title__, __released__);
  printf ("%s\n", __description__);
  printf ("%s\n\n", __author__);

#if defined(__GNUC__)
  printf ("Compiled by GNU CC version %s on %s %s\n",
	  __VERSION__, __DATE__, __TIME__);
#else
  printf ("Compiled by CC on %s : %s\n", __DATE__, __TIME__);
#endif /* __GNUC__ */
  printf ("%s-%s-%s (%s)\n", name . machine, name . sysname, name . release, name . nodename);
}
