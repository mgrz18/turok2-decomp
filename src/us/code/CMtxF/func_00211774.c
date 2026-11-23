#include "common.h"
#include "globals/engine.h"


/* Moves value toward target by rate * gFrameIncrement without overshooting. */
f32 Math_Approach(f32 value, f32 target, f32 rate) {
    if (value < target) {
        value += rate * gFrameIncrement;
        if (value > target) {
            value = target;
        }
    } else if (value > target) {
        value -= rate * gFrameIncrement;
        if (value < target) {
            value = target;
        }
    }
    return value;
}
