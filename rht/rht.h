/* The 'rht' is an abstract specification that defines an independent C interface for Hash Table implementations */

/*
 * Before using these header file applications need to define its own rht_t
 *
 * e.g. typedef struct rht rht_t;
 */

#pragma once

/* System headers */
#include <stdbool.h>

/* Project headers */
#include "rht-hashers.h"


/* Public Constants */
#define RHT_VERSION_MAJOR 0
#define RHT_VERSION_MINOR 1
#define RHT_VERSION_PATCH 0
#define RHT_VERSION       "0.1.0"


/* No public datatypes. The internals of the structure rht_t are all private */


/* typedef for hash table key/value pairs */
typedef char * rhtkey_t;                  /* keys are strings         */
typedef void * rhtval_t;                  /* values are void pointers */

/* typedef to iterate over hash table objects */
typedef void rht_each_f (void * obj);


#ifdef __cplusplus
extern "C" {
#endif


/* Public functions */

/* Versioning */
void rht_print_version (void);
const char * rht_lib_version (void);


/* API */
rht_t * rht_alloc (unsigned size);
void rht_clear (rht_t * ht);
void rht_free (rht_t * ht);
unsigned rht_count (rht_t * ht);
void rht_set (rht_t * ht, char * key, void * val);
void * rht_get (rht_t * ht, char * key);
void rht_del (rht_t * ht, char * key);
bool rht_has (rht_t * ht, char * key);
void rht_foreach (rht_t * ht, rht_each_f * fn, void * data);
char ** rht_keys (rht_t * ht);
void ** rht_vals (rht_t * ht);

#ifdef __cplusplus
}


class Rht {

 public:
  Rht (unsigned n);
  ~Rht ();

 private:
  void * ht = NULL;
};

#endif /* __cplusplus */
