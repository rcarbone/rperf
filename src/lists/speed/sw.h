
/* Public functions in file sw.c */
unsigned sw_maxname (sw_t * sw []);
unsigned sw_no (sw_t * sw []);
unsigned sw_have (sw_t * sw [], char * name);
rtime_t sw_call (sw_t * sw, char * name, unsigned items, relem_t * objs [], bool verbose);
rplugin_f * sw_func (sw_t * sw, char * name);
sw_t ** sw_init (char * argv [], unsigned itesm, bool verbose);
void sw_done (sw_t * sw [], bool verbose);
