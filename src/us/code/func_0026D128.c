#include "common.h"
#include "globals/engine.h"

/* Two floats packed as their upper 16 bits (see FloatToHalf / HalfToFloat). */
typedef struct {
    /* 0x0 */ u16 base;
    /* 0x2 */ u16 range;
} PackedRange;

s32 Demo_GetPlayerCount(void);
extern u32 gRandomValue; /* random seed A */

/* base + random(0..1) * range, both unpacked from half-floats. */
f32 RandomInPackedRange(PackedRange r) {
    u32 base = r.base << 16;
    u32 range;

    if (r.range == 0) {
        return *(f32 *) &base;
    }
    range = r.range << 16;
    if (Demo_GetPlayerCount()) {
        gRandomValue = gFrameCountGameplay * 0xA84A5B53 + 0x58348C2D;
    } else {
        gRandomValue = gRandomValue * 0xA84A5B53 + 0x58348C2D;
    }
    return *(f32 *) &base + (f32) ((gRandomValue >> 16) & 0x7FFF) * *(f32 *) &range * (1.0f / 32767);
}
