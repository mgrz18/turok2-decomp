typedef float f32;

extern f32 D_800A5688;          /* braking factor when reversing */
extern f32 D_800B6D28;          /* per-frame scale */

/* Moves value by delta, clamped to [-limit, limit]; reversing direction is damped. */
f32 func_0021160C(f32 value, f32 delta, f32 limit) {
    if (delta < 0.0f && value <= -limit) {
        return value;
    }
    if (delta > 0.0f && limit <= value) {
        return value;
    }
    if ((value < 0.0f && delta > 0.0f) || (value > 0.0f && delta < 0.0f)) {
        delta *= D_800A5688;
    }
    value += delta * D_800B6D28;
    if (delta < 0.0f) {
        if (value < -limit) {
            value = -limit;
        }
    } else if (limit < value) {
        value = limit;
    }
    return value;
}
