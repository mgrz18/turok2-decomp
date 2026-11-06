#include "common.h"

s32 Demo_GetPlayerCount(void);
extern u32 gFrameCountGameplay;
extern u32 gRandomValue; /* random seed A */

/* LCG step on seed A (re-seeded from gFrameCountGameplay when Demo_GetPlayerCount()); returns 0..0x7FFFFFFF. */
s32 Random_Next(void) {
    if (Demo_GetPlayerCount()) {
        gRandomValue = gFrameCountGameplay * 0xA84A5B53 + 0x58348C2D;
    } else {
        gRandomValue = gRandomValue * 0xA84A5B53 + 0x58348C2D;
    }
    return ((gRandomValue << 16) | (gRandomValue >> 16)) & 0x7FFFFFFF;
}
