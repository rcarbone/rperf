/* System headers */
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/* Private headers */
#include "varrays.h"
#include "scenari.h"


/* The local identifiers for the tests to run (indexed by keys) */
typedef enum
{
  T_GROW   = 0x00,    /* add N elements to an empty container                                               */
  T_HIT    = 0x01,    /* find all existing elements one-by-one (N elements in the container)                */
  T_MISS   = 0x02,    /* attempt to find all non-existing elements one-by-one (N elements in the container) */
  T_DELETE = 0x03,    /* remove all elements one-by-one from the container (N elements in the container)    */
  T_CHANGE = 0x04,    /* add N elements, overwrite their values in another loop                             */
  T_UDB    = 0x05,

} rtid_t;


/* All the test suite in an array */
static rtest_t builtins [] =
  {
    { T_GROW,   "grow",   "Add N elements to an empty container", NULL                             },
    { T_HIT,    "hit",    "Find N existing elements (N in the container)", NULL                    },
    { T_MISS,   "miss",   "Find with failure N elements (N in the container)", NULL                },
    { T_DELETE, "delete", "Delete N existing elements (N in the container)", NULL                  },
    { T_CHANGE, "change", "Replace (different key) N existing elements (N in the container)", NULL },
  };
#define T_TESTS (sizeof (builtins) / sizeof (* builtins))


/* Looukup for a test by id */
static rtest_t * rtlookup (unsigned id)
{
  unsigned i;
  for (i = 0; i < T_TESTS; i ++)
    {
      rtest_t * t = & builtins [i];
      if (t -> id == id)
	return t;
    }
  return NULL;
}


/* Return the # of tests defined */
unsigned rtno (void)
{
  return T_TESTS;
}


/* Looukup for a test name by id */
char * rtname (unsigned id)
{
  rtest_t * t = rtlookup (id);
  return t ? t -> name : NULL;
}


/* Looukup for a test description by id */
char * rtdescription (unsigned id)
{
  rtest_t * t = rtlookup (id);
  return t ? t -> description : NULL;
}


rtest_t * mktest (unsigned id, char * name, char * description, rmain_f * fun)
{
  rtest_t * t = calloc (1, sizeof (* t));

  t -> id          = id;
  t -> name        = strdup (name);
  t -> description = strdup (description);
  t -> fun         = fun;
  t -> enabled     = true;
  t -> min         = UINT64_MAX;

  return t;
}


void rmtest (void * test)
{
  rtest_t * t = test;
  if (! t)
    return;

  if (t -> name)
    free (t -> name);
  if (t -> description)
    free (t -> description);
  free (t);
}


rtest_t ** mktests (unsigned id, char * name, char * description, rmain_f * fun)
{
  rtest_t ** tests = NULL;
  unsigned i;
  for (i = 0; i < T_TESTS; i ++)
    tests = arrmore (tests, mktest (builtins [i] . id, builtins [i] . name, builtins [i] . description, builtins [i]. fun), rtest_t);
  return tests;
}


void rmtests (rtest_t * rargv [])
{
  arrclear (rargv, rmtest);
}
