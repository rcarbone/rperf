#pragma once

/* Project headers */
#include "rtypes.h"
#include "datasets.h"

unsigned rsuite_grow (unsigned argc, robj_t * argv []);
unsigned rsuite_hit (unsigned argc, robj_t * argv []);
unsigned rsuite_miss (unsigned argc, robj_t * argv []);
unsigned rsuite_delete (unsigned argc, robj_t * argv []);
unsigned rsuite_replace (unsigned argc, robj_t * argv []);
unsigned rsuite_kbench (unsigned argc, robj_t * argv []);

