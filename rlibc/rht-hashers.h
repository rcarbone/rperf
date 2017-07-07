
#pragma once

/*
 * The default hash functions for integers and strings.
 *
 * These functions are kept in a separate file to help applications that need
 * only such functionalities and nothing else related to rht definitions.
 *
 */


/* Stolen from java 1.4 source (as found in Niels Provos's hash table implementation) */
static inline unsigned rht_java_hash (unsigned h)
{
  h += ~(h << 9);
  h ^=  ((h >> 14) | (h << 18)); /* >>> */
  h +=  (h << 4);
  h ^=  ((h >> 10) | (h << 22)); /* >>> */
  return h;
}


/* Basic string hash function from Python (as found in Niels Provos's hash table implementation) */
static inline unsigned rht_python_hash (const char * s)
{
  unsigned h;
  const unsigned char * cp = (const unsigned char *) s;
  h = * cp << 7;
  while (* cp)
    h = (1000003 * h) ^ * cp ++;

  /* This conversion truncates the length of the string, but that's ok */
  h ^= (unsigned) (cp - (const unsigned char *) s);

  return h;
}
