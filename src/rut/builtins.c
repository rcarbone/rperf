/*
 * All the Unit Tests are here, grouped together in a static table.
 */


/* Project headers */
#include "rut.h"

/* Forward declarations */
static unsigned alloc_free (unsigned argc);
static unsigned alloc_add_free (unsigned argc);
static unsigned alloc_iterate_free (unsigned argc);
static unsigned alloc_count_free (unsigned argc);
static unsigned alloc_found_free (unsigned argc);
static unsigned alloc_miss_free (unsigned argc);
static unsigned alloc_delete_free (unsigned argc);
static unsigned alloc_missed_free (unsigned argc);


/* All the Unit Tests in a static table */
static rut_t builtins [] =
{
  { "make",    "Allocate and free",        alloc_free         },
  { "add",     "Add elements",             alloc_add_free     },
  { "iterate", "Iterate over existent",    alloc_iterate_free },
  { "count",   "Add elements and count",   alloc_count_free   },
  { "found",   "Search for existent",      alloc_found_free   },
  { "miss",    "Search for non-existent",  alloc_miss_free    },
  { "delete",  "Delete existent",          alloc_delete_free  },
  { "missed",  "Delete non-existent",      alloc_missed_free  },
};
#define RUTNO (sizeof (builtins) / sizeof (* builtins))


/* The implementation of the Unit Tests */


static unsigned alloc_free (unsigned argc)         { return argc; }
static unsigned alloc_add_free (unsigned argc)     { return argc; }
static unsigned alloc_iterate_free (unsigned argc) { return argc; }
static unsigned alloc_count_free (unsigned argc)   { return argc; }
static unsigned alloc_found_free (unsigned argc)   { return argc; }
static unsigned alloc_miss_free (unsigned argc)    { return argc; }
static unsigned alloc_delete_free (unsigned argc)  { return argc; }
static unsigned alloc_missed_free (unsigned argc)  { return argc; }



/* Return the # of Unit Tests */
unsigned rutargc (void)
{
  return RUTNO;
}


/* Return the table of the Unit Tests */
rut_t * rutargv (void)
{
  return builtins;
}
