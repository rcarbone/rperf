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
  SV * pval = newSVpv (val, sizeof (val));
  hv_store (ht, key, strlen (key), pval, rht_python_hash (key));
}


void * rht_get (rht_t * ht, char * key)
{
  SV ** pval = hv_fetch (ht, key, strlen (key), rht_python_hash (key));
  void * val = NULL;

  if (pval)
    {
      printf ("ROCCO\n");
      // val = SvPV (* pval, sizeof (void *));
      // val = SvPV (* pval, 8);
      SVPV (* pval, 8);
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
#if defined(ROCCO)
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    fn (data);
#endif /* ROCCO */
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
#if defined(ROCCO)
  unsigned i = 0;
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    keys [i ++] = (char *) apr_hash_this_key (k);
#endif /* ROCCO */
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
#if defined(ROCCO)
  unsigned i = 0;
  apr_hash_index_t * k;
  for (k = apr_hash_first (NULL, ht); k; k = apr_hash_next (k))
    vals [i ++] = (void *) apr_hash_this_val (k);
#endif /* ROCCO */
  return vals;
}
