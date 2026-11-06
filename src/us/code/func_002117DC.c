#include "common.h"

extern f32 gFrameIncrement; /* frame time scale */

/* Eases *value toward target by (target - *value) * rate * gFrameIncrement; snaps when rate is 1. */
void Math_EaseToward(f32 *value, f32 target, f32 rate) {
    f32 current;
    f32 step;

    current = *value;
    step = (target - current) * rate * gFrameIncrement;
    if (rate == 1.0f) {
        *value = target;
    } else if (current < target) {
        *value = current + step;
        if (*value > target) {
            *value = target;
        }
    } else if (current > target) {
        *value = current + step;
        if (*value < target) {
            *value = target;
        }
    }
}
