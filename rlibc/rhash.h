/* The 'rht' is an abstract specification that defines an independent C interface for Hash Table implementations */

/*
 * Before using these header file applications need to define its own rhash_t
 *
 * e.g. typedef struct rhash rhash_t;
 */

#pragma once

/* System headers */
#include <stdbool.h>

/* Project headers */
#include "rht-hashers.h"


/* Public Constants */
#define RHASH_VERSION_MAJOR 0
#define RHASH_VERSION_MINOR 1
#define RHASH_VERSION_PATCH 0
#define RHASH_VERSION       "0.1.0"


/* No public datatypes. The internals of the structure rhash_t are all private */


/* typedef for hash table key/value pairs */
typedef char * rhashkey_t;                  /* keys are strings         */
typedef void * rhashval_t;                  /* values are void pointers */

/* typedef to iterate over hash table objects */
typedef void rhash_each_f (void * obj);


#ifdef __cplusplus
extern "C" {
#endif


/* Public functions */

/* Versioning */
void rhash_print_version (void);
const char * rhash_lib_version (void);


/* API */
rhash_t * rhash_alloc (unsigned size);
void rhash_clear (rhash_t * ht);
void rhash_free (rhash_t * ht);
unsigned rhash_count (rhash_t * ht);
void rhash_set (rhash_t * ht, char * key, void * val);
void * rhash_get (rhash_t * ht, char * key);
void rhash_del (rhash_t * ht, char * key);
bool rhash_has (rhash_t * ht, char * key);
void rhash_foreach (rhash_t * ht, rhash_each_f * fn, void * data);
char ** rhash_keys (rhash_t * ht);
void ** rhash_vals (rhash_t * ht);

#ifdef __cplusplus
}


class Rhash {

 public:
  Rhash (unsigned n);
  ~Rhash ();

 private:
  void * ht = NULL;
};

#endif /* __cplusplus */
