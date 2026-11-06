#include "common.h"
#include "m2c_macros.h"

extern s32 RumbleCurve_Advance(void *);

s32 Rumble_IsFinished(u8 *arg0) {
    s32 r = 0;

    if (RumbleCurve_Advance(arg0 + 0x20) != 0 || RumbleCurve_Advance(arg0 + 0x2C) != 0) {
        r = 1;
    }
    return r;
}
