#include "common.h"
#include "globals/math.h"
#include "globals/engine.h"

s32 Demo_GetPlayerCount(void);

/* LCG step on seed B (re-seeded from gFrameCountGameplay when Demo_GetPlayerCount()); returns 0..0x7FFFFFFF. */
s32 Random_NextB(void) {
    if (Demo_GetPlayerCount()) {
        gRandomValueB = gFrameCountGameplay * 0xA84A5B53 + 0x58348C2D;
    } else {
        gRandomValueB = gRandomValueB * 0xA84A5B53 + 0x58348C2D;
    }
    return ((gRandomValueB << 16) | (gRandomValueB >> 16)) & 0x7FFFFFFF;
}
