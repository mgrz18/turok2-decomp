typedef signed int s32;
typedef float f32;

extern f32 gFrameIncrement;

s32 Timer_Countdown(f32 *timer) {
    f32 t = *timer - gFrameIncrement;
    *timer = t;
    if (t <= 0.0f) {
        *timer = 0.0f;
        return 1;
    }
    return 0;
}
