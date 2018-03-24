/* System headers */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


/* Project headers */
#include "rnodes.h"
#include "safe.h"
#include "support.h"


rnode_t * mknode (unsigned x)
{
  rnode_t * node = (rnode_t *) calloc (1, sizeof (* node));

  node -> key = strdup (utoa (x));
  node -> val = x;
  node -> foo = node;
  node -> len = strlen (node -> key);

  return node;
}


void rmnode (rnode_t * node)
{
  if (! node)
    return;

  safefree (node -> key);
  free (node);
}


int cmpnode (const void * n1, const void * n2)
{
  return strcmp (((rnode_t *) n1) -> key, ((rnode_t *) n2) -> key);
}


/* Initializes the array with the natural numbers from 1 to argc */
rnode_t ** mknodes (unsigned argc)
{
  rnode_t ** argv = (rnode_t **) calloc (argc + 1, sizeof (* argv));
  unsigned i;

  for (i = 0; i < argc; i ++)
    argv [i] = mknode (i + 1);
  argv [i] = NULL;

  return argv;
}


void rmnodes (rnode_t * argv [])
{
  rnode_t ** e = argv;
  while (e && * e)
    rmnode (* e ++);
  safefree (argv);
}


/* Initializes the array with the natural numbers from argc + 1 to 2 * argc */
rnode_t ** mkmiss (unsigned argc)
{
  rnode_t ** argv = (rnode_t **) calloc (argc + 1, sizeof (* argv));
  unsigned i;

  for (i = 0; i < argc; i ++)
    argv [i] = mknode (argc + i + 1);
  argv [i] = NULL;

  return argv;
}
