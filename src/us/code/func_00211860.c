#include "common.h"
#include "globals/engine.h"

/* De-faked in round 21 (j4): the draft returned through two identical arms under a test of the limit;
 * the plain clamp matches. */

/* Steps value by step per frame (scaled by gFrameIncrement) and stops it at limit: at -limit when
 * stepping down. */
s32 Math_StepClampedInt(s32 value, s32 step, s32 limit) {
    f32 rate = step;

    value += (s32) (rate * gFrameIncrement);
    if (rate < 0.0f) {
        limit = -limit;
        if (value < limit) {
            value = limit;
        }
    } else if (value > limit) {
        value = limit;
    }
    return value;
}
