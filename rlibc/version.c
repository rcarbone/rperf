/* System headers */
#include <stdio.h>
#include <limits.h>
#include <sys/utsname.h>


/* librhash - an abstract C library over real hash tables */
typedef struct rhash rhash_t;
#include "rhash.h"


/* The name of the game with its version number (major), release number (minor) and patchlevel (lower) */
static char __pkgname__     [] = "librhash";
static char __version__     [] = "version "RHASH_VERSION;
static char __title__       [] = "hash tables without a hash table";
static char __released__    [] = "May 2017";
static char __description__ [] = "librhash - a C library for virtualization of hash tables";
static char __author__      [] = "Rocco Carbone <rocco@tecsiel.it>";


/* The string version */
static const char rhash_version_string [] = "librhash version "RHASH_VERSION;


const char * rhash_lib_version (void)
{
  return rhash_version_string;
}


/* Print software information */
void rhash_print_version (void)
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

#if defined(ROCCO)
  printf ("\nC Pre-processor defines:\n");

  /* Operating Systems */
  printf ("\n\n");
#endif /* ROCCO */
}
