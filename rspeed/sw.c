/* Projects headers */
#include "safe.h"
#include "args.h"
#include "rtest.h"
#include "rspeed.h"


/* Get the 'module' variable defined in the plugin */
static char * pmodule (rplugin_t * p)
{
  char * m = rplugin_module (p);
  return m ? m : "undefined";
}


static rtest_t * mktest (unsigned id, char * name, char * description, rplugin_f * fun)
{
  rtest_t * t = calloc (1, sizeof (* t));

  t -> id          = id;
  t -> name        = strdup (name);
  t -> description = strdup (description);

  return t;
}


static void rmtest (void * _t)
{
  rtest_t * t = _t;
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
      n = RMAX (strlen ((* sw) -> name), n);
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


/* Returns number of implementations for the given name */
unsigned sw_have (sw_t * sw [], char * name)
{
  unsigned n = 0;
  while (name && sw && * sw)
    if (sw_func (* sw ++, name))
      n ++;
  return n;
}


/* Return the pointer to the test with the given id (if any) for the given implementation */
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


rtime_t sw_call (sw_t * sw, char * name, unsigned items, robj_t * objs [], bool verbose)
{
  /* Lookup for a function implemented in the shared object having the given name */
  rplugin_f * fun = sw_func (sw, name);
  rtime_t t1;
  rtime_t t2;
  if (fun)
    {
      int argc = 0;
      char * argv [5] = { NULL, NULL, NULL, NULL, NULL };
      char aitems [32];
      bool ret;

      sprintf (aitems, "%u", items);

      argv [argc ++] = name;
      argv [argc ++] = "-n";
      argv [argc ++] = aitems;

      if (verbose)
	argv [argc ++] = "-v";

      if (verbose)
	printf ("    calling %s/%s ... ", pmodule (sw -> plugin), name);

      t1 = nswall ();
      ret = fun (argc, argv, (void **) objs);
      t2 = nswall ();

      if (verbose)
	{
	  if (! ret)
	    printf (" Ok\n");
	  else
	    printf (" No\n");
	}
      return ! ret ? t2 - t1 : 0;
    }
  return 0;
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
  arrclear (sw -> suite, rmtest);
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
      char ** funcs = rplugin_functions (sw -> plugin);
      char ** f = funcs;
      unsigned i = 0;

      /* Call now its boot() function */
      sw_call (sw, "boot", 0, NULL, verbose);

      sw -> pathname = strdup (pathname);
      sw -> name     = strdup (pmodule (sw -> plugin));

      while (f && * f)
	{
	  sw -> suite = arrmore (sw -> suite, mktest (i ++, * f, * f, sw_func (sw, * f)), rtest_t);
	  f ++;
	}
      argsclear (funcs);
    }
  else
    {
      safefree (buffer);
      sw = rmsw (sw);
    }

  return sw;
}


/* Initialize all the implementations under test
 *   - load the shared object in memory
 *   - call its boot() function
 */
sw_t ** sw_init (char * argv [], unsigned itesm, bool verbose)
{
  char ** path = argv;
  sw_t ** done = NULL;

  if (verbose)
    {
      printf ("Initializing ... ");
      printf ("\n\n");
    }

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
#if defined(ROCCO)
	  else
	    printf (".");
#endif /* ROCCO */

          done = arrmore (done, sw, sw_t);
	}
      else
	{
	  if (verbose)
	    printf ("No\n");
#if defined(ROCCO)
	  else
            printf ("x");
#endif /* ROCCO */
	}

      path ++;
    }

  if (verbose)
    printf ("\n");

  printf (" %d implementations initialized (of %d defined)\n\n", arrlen (done), arrlen (argv));

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

  if (verbose)
    {
      printf ("Terminating  ... ");
      printf ("\n\n");
    }

  while (sw && * sw)
    {
      /* Call the halt function */
      sw_call (* sw, "halt", 0, NULL, verbose);

      rmsw (* sw ++);

      if (verbose)
	printf (".");

      done ++;
    }

  if (verbose)
    printf (" %d implementations terminated (of %d loaded)\n", done, arrlen (argv));

  safefree (argv);
}
