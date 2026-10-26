#include "common.h"

extern f32 D_800B6D28; /* frame time scale */

/* Moves value toward target by rate * D_800B6D28 without overshooting. */
f32 func_00211774(f32 value, f32 target, f32 rate) {
    if (value < target) {
        value += rate * D_800B6D28;
        if (value > target) {
            value = target;
        }
    } else if (value > target) {
        value -= rate * D_800B6D28;
        if (value < target) {
            value = target;
        }
    }
    return value;
}
