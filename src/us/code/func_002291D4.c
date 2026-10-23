typedef int s32;
typedef unsigned char u8;

/* Darkens an RGB colour by percent (0 = unchanged). */
void func_002291D4(u8 percent, u8 r, u8 g, u8 b, u8 *outR, u8 *outG, u8 *outB) {
    s32 keep;

    if (percent != 0) {
        keep = 100 - percent;
        *outR = r * 0.01f * keep;
        *outG = g * 0.01f * keep;
        *outB = b * 0.01f * keep;
    } else {
        *outR = r;
        *outG = g;
        *outB = b;
    }
}
