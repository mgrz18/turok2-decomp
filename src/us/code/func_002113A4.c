#include "common.h"
#include "globals/math.h"
#include "globals/engine.h"

s32 Demo_GetPlayerCount(void);

/* LCG step on seed A (re-seeded from gFrameCountGameplay when Demo_GetPlayerCount()); returns 0..0x7FFFFFFF. */
s32 Random_Next(void) {
    if (Demo_GetPlayerCount()) {
        gRandomValue = gFrameCountGameplay * 0xA84A5B53 + 0x58348C2D;
    } else {
        gRandomValue = gRandomValue * 0xA84A5B53 + 0x58348C2D;
    }
    return ((gRandomValue << 16) | (gRandomValue >> 16)) & 0x7FFFFFFF;
}
