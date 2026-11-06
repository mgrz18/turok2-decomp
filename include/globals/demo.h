#ifndef GLOBALS_DEMO_H
#define GLOBALS_DEMO_H

/* The attract-mode demos Demo_PlayNext plays in turn (structs/demo.h), a
 * NULL-terminated list in the first VM module's data, and its module-2 copy. */

#include "structs/demo.h"

extern DemoHeader *gDemoTable[];
extern DemoHeader *gDemoTable_2[];

/* Fields of gDemoPlayer with symbols of their own */
extern s32 gDemoPlayer_fastForward;                       /* gDemoPlayer.fastForward */
extern void (*gDemoPlayer_match_decode)(DemoStream *stream);  /* gDemoPlayer.match.decode */

#endif
