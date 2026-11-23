#include "common.h"
#include "globals/engine.h"


/* Moves value by delta, clamped to [-limit, limit]; reversing direction is damped. */
f32 Math_StepClamped(f32 value, f32 delta, f32 limit) {
    if (delta < 0.0f && value <= -limit) {
        return value;
    }
    if (delta > 0.0f && limit <= value) {
        return value;
    }
    if ((value < 0.0f && delta > 0.0f) || (value > 0.0f && delta < 0.0f)) {
        delta *= 1.5f;
    }
    value += delta * gFrameIncrement;
    if (delta < 0.0f) {
        if (value < -limit) {
            value = -limit;
        }
    } else if (limit < value) {
        value = limit;
    }
    return value;
}
