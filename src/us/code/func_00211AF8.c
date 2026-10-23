typedef signed int s32;
typedef float f32;

extern f32 D_800B6D28;

s32 func_00211AF8(f32 *timer) {
    f32 t = *timer - D_800B6D28;
    *timer = t;
    if (t <= 0.0f) {
        *timer = 0.0f;
        return 1;
    }
    return 0;
}
