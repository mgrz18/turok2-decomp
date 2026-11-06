typedef int s32;
typedef float f32;


f32 _getVol(f32 value, s32 shift, s32 whole, s32 frac) {
    s32 n = shift >> 3;
    s32 i = 0;
    f32 base;
    f32 result;

    if (n == 0) {
        return value;
    }
    base = ((f32)(whole << 16) + (f32)(frac & 0xFFFF)) * 1.5258789e-5f;
    result = 1.0f;
    do {
        if (n & 1) {
            result *= base;
        }
        n >>= 1;
        if (n == 0) {
            break;
        }
        i++;
        base *= base;
    } while (i < 32);
    return value * result;
}
