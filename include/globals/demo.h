#ifndef GLOBALS_DEMO_H
#define GLOBALS_DEMO_H

/* The attract-mode demos Demo_PlayNext plays in turn (structs/demo.h), a
 * NULL-terminated list in the first VM module's data, and its module-2 copy. */

#include "structs/demo.h"

extern DemoHeader *gDemoTable[];
extern DemoHeader *gDemoTable_2[];

#endif
