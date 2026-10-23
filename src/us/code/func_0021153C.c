typedef signed int s32;
typedef float f32;

/* acos lookup: 1024 samples of acos over [0, 1]. */
extern f32 D_800AB8D0[];

f32 func_0021153C(f32 x) {
    s32 neg;
    f32 pos;
    f32 lo;
    f32 r;
    s32 i;

    if (x < 0.0f) {
        neg = 1;
        pos = x * -1023.0f;
    } else {
        pos = x * 1023.0f;
        neg = 0;
    }
    i = pos;
    if ((f32) i < 1023.0f) {
        lo = D_800AB8D0[i];
        r = lo + (pos - (f32) i) * (*(&D_800AB8D0[i] + 1) - lo);
        if (neg) {
            return 3.1415927f - r;
        }
        return r;
    }
    if (neg) {
        return 3.1415927f - D_800AB8D0[1023];
    }
    return D_800AB8D0[1023];
}
