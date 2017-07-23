/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include "EASTL/hash_map.h"
#include "hashtable.cpp"
using namespace eastl;

/* operator new required by EASTL */
#define THROW_SPEC_0    // Throw 0 arguments
#define THROW_SPEC_1(x) // Throw 1 argument

/* Stolen from doc/SampleNewAndDelete.cpp */
void * operator new [] (size_t size, const char* /*name*/, int /*flags*/, 
			unsigned /*debugFlags*/, const char* /*file*/, int /*line*/) THROW_SPEC_1(std::bad_alloc)
{
  return calloc (1, size);
}


void * operator new [] (size_t size, size_t alignment, size_t alignmentOffset, const char* /*name*/, 
			int flags, unsigned /*debugFlags*/, const char* /*file*/, int /*line*/) THROW_SPEC_1(std::bad_alloc)
{
  // Substitute your aligned malloc. 
#define malloc_aligned(x, y, z) calloc(1, x)
  return malloc_aligned(size, alignment, alignmentOffset);
}


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

/* librhash - an abstract C library over real hash tables */
#include "rht-hashers.h"

/* Our own specialized hashing function in order to avoid the performance depends on the hash implementation used */
class hashfunc
{
 public:
  unsigned operator () (char * key) const { return rht_python_hash (key); }
};

typedef eastl::hash_map<char *, void *, hashfunc> rht_t;
#include "rht.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

rht_t * rht_alloc (unsigned size)
{
  return new rht_t;
}


void rht_free (rht_t * ht)
{
  delete ht;
}


void rht_clear (rht_t * ht)
{
  ht -> clear ();
}


unsigned rht_count (rht_t * ht)
{
  return ht -> size ();
}


void rht_set (rht_t * ht, char * key, void * val)
{
  ht -> insert (rht_t::value_type (key, val));
}


void * rht_get (rht_t * ht, char * key)
{
  rht_t::iterator hit = ht -> find (key);
  return hit != ht -> end () ? hit -> second : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  ht -> erase (key);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  rht_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = (char **) calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  rht_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    keys [i ++] = it -> first;
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = (void **) calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  rht_t::iterator it;
  for (it = ht -> begin (); it != ht -> end (); it ++)
    vals [i ++] = it -> second;
  return vals;
}
