#include "common.h"

extern f32 D_800B6D28; /* frame time scale */

/* Moves value toward zero by rate * D_800B6D28 without overshooting. */
f32 func_0021170C(f32 value, f32 rate) {
    if (value > 0.0f) {
        value -= rate * D_800B6D28;
        if (value < 0.0f) {
            value = 0.0f;
        }
    } else if (value < 0.0f) {
        value += rate * D_800B6D28;
        if (value > 0.0f) {
            value = 0.0f;
        }
    }
    return value;
}
