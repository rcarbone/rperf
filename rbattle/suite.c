/* Project headers */
#include "rbattle.h"


/* The local identifiers for the tests to run (indexed by keys) */
typedef enum
{
  T_GROW   = 0x00,    /* add N elements to an empty container                                               */
  T_HIT    = 0x01,    /* find all existing elements one-by-one (N elements in the container)                */
  T_MISS   = 0x02,    /* attempt to find all non-existing elements one-by-one (N elements in the container) */
  T_DELETE = 0x03,    /* remove all elements one-by-one from the container (N elements in the container)    */
  T_CHANGE = 0x04,    /* add N elements, overwrite their values in another loop                             */
  T_UDB    = 0x05,

} tid_t;


/* All the test suite in an array */
static test_t suite [] =
  {
    { T_GROW,   "grow",   "Add N elements to an empty container"                             },
    { T_HIT,    "hit",    "Find N existing elements (N in the container)"                    },
    { T_MISS,   "miss",   "Find with failure N elements (N in the container)"                },
    { T_DELETE, "delete", "Delete N existing elements (N in the container)"                  },
    { T_CHANGE, "change", "Replace (different key) N existing elements (N in the container)" },
  };
#define T_TESTS (sizeof (suite) / sizeof (* suite))


/* Looukup for a test by id */
static test_t * tlookup (unsigned id)
{
  unsigned i;
  for (i = 0; i < T_TESTS; i ++)
    {
      test_t * t = & suite [i];
      if (t -> id == id)
	return t;
    }
  return NULL;
}


/* Return the # of tests defined */
unsigned tno (void)
{
  return T_TESTS;
}


/* Looukup for a test name by id */
char * tname (unsigned id)
{
  test_t * t = tlookup (id);
  return t ? t -> name : NULL;
}


/* Looukup for a test description by id */
char * tdescription (unsigned id)
{
  test_t * t = tlookup (id);
  return t ? t -> description : NULL;
}
