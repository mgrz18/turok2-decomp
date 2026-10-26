#include "common.h"

extern f32 D_800B6D28; /* frame time scale */

/* Eases *value toward target by (target - *value) * rate * D_800B6D28; snaps when rate is 1. */
void func_002117DC(f32 *value, f32 target, f32 rate) {
    f32 current;
    f32 step;

    current = *value;
    step = (target - current) * rate * D_800B6D28;
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
