/* System headers */
#include <stdio.h>

/* The implementation */
#include <Python.h>

/* librhash - an abstract C library over real hash tables */
typedef PyObject rht_t;
#include "rht-api.h"


/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=- */


rht_t * rht_alloc (unsigned size)
{
  Py_Initialize ();
  return PyDict_New ();
}


void rht_free (rht_t * ht)
{
  PyDict_Clear (ht);
  Py_Finalize ();
}


void rht_clear (rht_t * ht)
{
  PyDict_Clear (ht);
}


unsigned rht_count (rht_t * ht)
{
  return PyDict_Size (ht);
}


void rht_set (rht_t * ht, char * key, void * val)
{
  PyDict_SetItem (ht, PyString_FromString (key), PyLong_FromVoidPtr (val));
}


void * rht_get (rht_t * ht, char * key)
{
  PyObject * hit = PyDict_GetItem (ht, PyString_FromString (key));
  return hit ? PyLong_AsVoidPtr (hit) : NULL;
}


void rht_del (rht_t * ht, char * key)
{
  PyDict_DelItem (ht, PyString_FromString (key));
}


bool rht_has (rht_t * ht, char * key)
{
  return rht_get (ht, key);
}


void rht_foreach (rht_t * ht, rht_each_f * fn, void * data)
{
  Py_ssize_t pos = 0;
  PyObject * key;
  PyObject * value;

  while (PyDict_Next (ht, & pos, & key, & value))
    fn (data);
}


char ** rht_keys (rht_t * ht)
{
  char ** keys = calloc (rht_count (ht) + 1, sizeof (char *));
  unsigned i = 0;
  Py_ssize_t pos = 0;
  PyObject * key;
  PyObject * value;
  while (PyDict_Next (ht, & pos, & key, & value))
    keys [i ++] = PyString_AsString (key);
  return keys;
}


void ** rht_vals (rht_t * ht)
{
  void ** vals = calloc (rht_count (ht) + 1, sizeof (void *));
  unsigned i = 0;
  Py_ssize_t pos = 0;
  PyObject * key;
  PyObject * val;
  while (PyDict_Next (ht, & pos, & key, & val))
    vals [i ++] = PyLong_AsVoidPtr (val);
  return vals;
}
