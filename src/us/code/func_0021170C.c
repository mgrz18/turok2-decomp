#include "common.h"

extern f32 gFrameIncrement; /* frame time scale */

/* Moves value toward zero by rate * gFrameIncrement without overshooting. */
f32 Math_DecayToZero(f32 value, f32 rate) {
    if (value > 0.0f) {
        value -= rate * gFrameIncrement;
        if (value < 0.0f) {
            value = 0.0f;
        }
    } else if (value < 0.0f) {
        value += rate * gFrameIncrement;
        if (value > 0.0f) {
            value = 0.0f;
        }
    }
    return value;
}
