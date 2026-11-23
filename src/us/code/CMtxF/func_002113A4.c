#include "common.h"
#include "globals/math.h"
#include "globals/engine.h"

s32 Demo_GetPlayerCount(void);
#ifdef TARGET_PORT
void T2Port_RngTrace(void *ra); /* DEBUG-ONLY: T2_RNG_TRACE (port/game/t2det.c) */
#endif

/* LCG step on seed A (re-seeded from gFrameCountGameplay when Demo_GetPlayerCount()); returns 0..0x7FFFFFFF. */
s32 Random_Next(void) {
#ifdef TARGET_PORT
    T2Port_RngTrace(__builtin_return_address(0));
#endif
    if (Demo_GetPlayerCount()) {
        gRandomValue = gFrameCountGameplay * 0xA84A5B53 + 0x58348C2D;
    } else {
        gRandomValue = gRandomValue * 0xA84A5B53 + 0x58348C2D;
    }
    return ((gRandomValue << 16) | (gRandomValue >> 16)) & 0x7FFFFFFF;
}
