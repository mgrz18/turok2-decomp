typedef int s32;
typedef float f32;

extern f32 D_800AA648;
extern f32 D_800AA64C;

f32 func_002961E8(f32 value, s32 shift, s32 whole, s32 frac) {
    s32 n = shift >> 3;
    s32 i = 0;
    f32 base;
    f32 result;

    if (n == 0) {
        return value;
    }
    base = ((f32)(whole << 16) + (f32)(frac & 0xFFFF)) * D_800AA648;
    result = D_800AA64C;
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
