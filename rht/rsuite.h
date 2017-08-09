#pragma once

/* Project headers */
#include "rtypes.h"
#include "datasets.h"

rtime_t rsuite_grow (unsigned argc, robj_t * argv []);
rtime_t rsuite_hit (unsigned argc, robj_t * argv []);
rtime_t rsuite_miss (unsigned argc, robj_t * argv []);
rtime_t rsuite_delete (unsigned argc, robj_t * argv []);
rtime_t rsuite_replace (unsigned argc, robj_t * argv []);
rtime_t rsuite_kbench (unsigned argc, robj_t * argv []);

