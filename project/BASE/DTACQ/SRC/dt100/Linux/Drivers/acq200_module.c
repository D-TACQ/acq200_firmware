/*
 * total hack to wrap acq32.c as acq200
 */

extern int acq200_init_module(void);
extern void acq200_cleanup_module(void);



#define ACQ200
#define PROCFILENAME "acq200"
//#define PROCFILENAME "acq32"

#include "acq32.c"
