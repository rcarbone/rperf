/* System headers */
#include <stdio.h>
#include <stdlib.h>

/* The implementation */
#include <EXTERN.h>
#include <perl.h>

/* librht - an abstract C library over real hash tables */
typedef HV rht_t;
#include "rht.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */

static PerlInterpreter * my_perl = NULL;

rht_t * rht_alloc (unsigned size)
{
  my_perl = perl_alloc ();
  HV * ht;

  perl_construct (my_perl);
  ht = newHV ();

  return ht;
}


void rht_free (rht_t * ht)
{
  hv_clear (ht);
  perl_destruct (my_perl);
  perl_free (my_perl);
  my_perl = NULL;
}


void rht_clear (rht_t * ht)
{
  hv_clear (ht);
}


unsigned rht_count (rht_t * ht)
{
  return HvKEYS (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  SV * pval = newSVsv (val);
  hv_store (ht, key, strlen (key), pval, rht_python_hash (key));
}


void * rht_get (rht_t * ht, char * key)
{
  SV ** hit = hv_fetch (ht, key, strlen (key), rht_python_hash (key));
  void * val = NULL;
  if (hit)
    {
      val = SvPV_nolen (* hit);
      // return SvPV (* hit, sizeof (void *));
      return val;
    }
  return val;
}


void rht_del (rht_t * ht, char * key)
{
  hv_delete (ht, key, strlen (key), G_DISCARD);
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
#if defined(ROCCO)
  unsigned i = 0;
  keys [i ++] = NULL;
#endif /* ROCCO */
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
#if defined(ROCCO)
  unsigned i = 0;
    vals [i ++] = NULL;
#endif /* ROCCO */
  return vals;
}
