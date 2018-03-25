#pragma once

/* System headers */
#include <stdint.h>


/* Typedefs that rlibc needs */


/* The wall time in nsecs */
typedef uint64_t rtime_t;

/* The type definition for a test function */
typedef rtime_t rmain_f (unsigned argc, void * argv []);

