#pragma once

/* System headers */
#include <getopt.h>

unsigned optmax (struct option * options);
char * optlegitimate (struct option * options);
char * optname (struct option * options, unsigned n, unsigned val);
void usage_item (struct option * options, unsigned n, unsigned val, char * description);
