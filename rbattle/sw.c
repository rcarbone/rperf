/* Public header */
#include "rbattle.h"

/* Check if an implementation has defined a function with a given name */
static char * pname (rplugin_t * p)
{
  if (p && p -> funs)
    {
      rplugin_symbol_t ** v = p -> vars;
      while (v && * v)
        {
          if (! strcmp ("module", (* v) -> name))
	    return rplugin_variable ((* v) -> name, p -> vars);
          v ++;
        }
    }
  return "undefined";
}


static test_t * mktest (unsigned id, char * name, char * description, rplugin_f * fun)
{
  test_t * t = calloc (1, sizeof (test_t));

  t -> id          = id;
  t -> name        = strdup (name);
  t -> description = strdup (description);
  t -> fun         = fun;
  t -> enabled     = true;

  /* Cleaunp test counters */
  t -> min        = 0xffffffff;     /* Shortest time spent */
  t -> avg        = 0.0;            /* Average time spent  */
  t -> max        = 0.0;            /* Longest time spent  */
  t -> rate       = 0;

  return t;
}


static void rmtest (void * _t)
{
  test_t * t = _t;
  if (! t)
    return;

  if (t -> name)
    free (t -> name);
  if (t -> description)
    free (t -> description);
  free (t);
}


/* Evaluate max length of implementation names under test */
unsigned sw_maxname (sw_t * sw [])
{
  unsigned n = 0;
  while (sw && * sw)
    {
      if ((* sw) -> enabled && (* sw) -> name)
	n = MAX (strlen ((* sw) -> name), n);
      sw ++;
    }
  return n;
}


/* Evaluate # of implementations under test */
unsigned sw_no (sw_t * sw [])
{
  unsigned n = 0;
  while (sw && * sw ++)
    n ++;
  return n;
}


/* Return the pointer to the test with the given id (if any) for the given implementation */
/* Check if an implementation has defined a function with a given name */
rplugin_f * sw_func (sw_t * sw, char * name)
{
  if (sw && sw -> plugin && sw -> plugin -> funs)
    {
      rplugin_symbol_t ** f = sw -> plugin -> funs;
      while (f && * f)
        {
          if (! strcmp (name, (* f) -> name))
	    return rplugin_function ((* f) -> name, sw -> plugin -> funs);
          f ++;
        }
    }
  return NULL;
}


bool sw_call (sw_t * sw, char * name, unsigned items, robj_t * objs [], bool verbose)
{
  /* Lookup for a function implemented in the shared object having the given name */
  rplugin_f * fun = sw_func (sw, name);
  if (fun)
    {
      int argc = 0;
      char * argv [5] = { NULL, NULL, NULL, NULL, NULL };
      int n = strlen (pname (sw -> plugin));
      char aitems [32];
      bool ret;

      sprintf (aitems, "%u", items);

      argv [argc ++] = name;
      argv [argc ++] = "-n";
      argv [argc ++] = aitems;

      if (verbose)
	argv [argc ++] = "-v";

      if (verbose)
	printf ("    calling %-*.*s/%s ... ", n, n, pname (sw -> plugin), name);

      ret = fun (argc, argv, (void **) objs);

      if (verbose)
	{
	  if (! ret)
	    printf (" Ok\n");
	  else
	    printf (" No\n");
	  return ! ret;
	}
    }
  return false;
}


static sw_t * rmsw (sw_t * sw)
{
  if (! sw)
    return NULL;

  if (sw -> pathname)
    free (sw -> pathname);
  if (sw -> name)
    free (sw -> name);
  rplugin_rm (sw -> plugin);
  vaclear ((void **) sw -> suite, rmtest);
  free (sw);

  return NULL;
}


static sw_t * mksw (char * pathname, bool verbose)
{
  sw_t * sw = calloc (1, sizeof (sw_t));
  int error = 0;
  char * buffer = NULL;

  /* Load the shared object in memory */
  sw -> plugin = rplugin_mk (pathname, & error, & buffer);
  if (sw -> plugin)
    {
      unsigned i = 0;

      /* Call now its boot() function */
      sw_call (sw, "boot", 0, NULL, verbose);

      sw -> enabled  = true;
      sw -> pathname = strdup (pathname);
      sw -> name     = strdup (pname (sw -> plugin));

      for (i = 0; i < tno (); i ++)
	{
	  rplugin_f * fun = sw_func (sw, tname (i));
	  if (fun)
	    {
	      sw -> suite = (test_t **) vamore ((void **) sw -> suite, mktest (i, tname (i), tdescription (i), fun));
	    }
	}
    }
  else
    {
      sw = rmsw (sw);
      free (buffer);
    }

  return sw;
}


/* Initialize all the implementations under test
 *   - load the shared object
 *   - call its boot() function
 */
sw_t ** sw_init (char * argv [], unsigned itesm, bool verbose)
{
  char ** path = argv;
  sw_t ** done = NULL;

  printf ("Initializing ... ");
  if (verbose)
    printf ("\n\n");

  while (path && * path)
    {
      sw_t * sw;

      if (verbose)
	printf ("  loading %s ... ", * path);

      sw = mksw (* path, verbose);
      if (sw)
	{
	  if (verbose)
	    printf ("Ok\n");
	  else
	    printf (".");

          done = (sw_t **) vamore ((void **) done, sw);
	}
      else
	{
	  if (verbose)
	    printf ("No\n");
	  else
            printf ("X");
	}

      path ++;
    }

  if (verbose)
    printf ("\n");

  printf (" %d implementations initialized (of %d defined)\n", valen ((void **) done), valen ((void **) argv));

  return done;
}


/* Terminate all the implementations under test
 *   - call its halt() function
 *   - unload the shared object
 */
void sw_done (sw_t * argv [], bool verbose)
{
  sw_t ** sw = argv;
  unsigned done = 0;

  printf ("\n");
  printf ("Terminating  ... ");
  if (verbose)
    printf ("\n\n");

  while (sw && * sw)
    {
      /* Call the halt function */
      sw_call (* sw, "halt", 0, NULL, verbose);

      rmsw (* sw ++);

      printf (".");

      done ++;
    }

  printf (" %d implementations terminated (of %d loaded)\n", done, valen ((void **) argv));

  if (argv)
    free (argv);
}


test_t * get_test (sw_t * sw, unsigned id)
{
  unsigned t;

  if (! sw -> enabled || ! sw -> suite)
    return NULL;

  for (t = 0; t < tno (); t ++)
    if (sw -> suite [t] && sw -> suite [t] -> id == id && sw -> suite [t] -> fun && sw -> suite [t] -> enabled)
      return sw -> suite [t];
  return NULL;
}


/* Returns number of implementations for the given id */
unsigned implemented (sw_t * sw [], unsigned id)
{
  unsigned n = 0;
  while (sw && * sw)
    if (sw_func (* sw ++, tname (id)))
      n ++;
  return n;
}
