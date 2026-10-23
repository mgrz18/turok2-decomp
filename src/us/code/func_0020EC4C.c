typedef float f32;

static __inline__ f32 wrapAngle(f32 angle) {
    while (angle < -3.1415927f) {
        angle += 6.2831855f;
    }
    while (angle > 3.1415927f) {
        angle -= 6.2831855f;
    }
    return angle;
}

static __inline__ f32 angleDelta(f32 a, f32 b) {
    return wrapAngle(a - wrapAngle(b));
}

#define ABS(x) (((x) < 0.0f) ? -(x) : (x))

/* Absolute difference between two angles, in [0, pi]. */
f32 func_0020EC4C(f32 a, f32 b) {
    return ABS(-angleDelta(a, b));
}
