#include "common.h"
#include "globals/math.h"
#include "globals/engine.h"

s32 Demo_GetPlayerCount(void);

/* Random blend between a and b (t in [0, 1] from seed A, steps of 1/9999). */
f32 Random_Blend(f32 a, f32 b) {
    f32 t;

    if (Demo_GetPlayerCount()) {
        gRandomValue = gFrameCountGameplay * 0xA84A5B53 + 0x58348C2D;
    } else {
        gRandomValue = gRandomValue * 0xA84A5B53 + 0x58348C2D;
    }
    t = ((s32) (((gRandomValue << 16) | (gRandomValue >> 16)) & 0x7FFFFFFF) % 10000) * (1.0f / 9999.0f);
    return t * a + (1.0f - t) * b;
}
