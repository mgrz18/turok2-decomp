/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/sdk/lib_802BB630/802BBC50.CPP, func_802B6B80_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern float D_800AA7C0;
extern float D_800AA7C4;
void func_00297AE0(s32 *arg0) {
    f32 matrix[16];
    u8 *diagBase;
    u8 *diag;
    u8 *elem;
    u8 *row;
    s32 r;
    s32 c;
    f32 one;
    s32 *outLo;
    s32 *outHi;
    f32 *inRow;
    f32 *inPair;
    f32 mult;
    u32 mask;
    f32 t1;
    f32 t0;
    s32 v0;
    s32 v1;
    s32 i;
    s32 j;
    r = 0;
    one = D_800AA7C0;
    diagBase = (u8 *)matrix;
    row = (u8 *)matrix;
    do {
        c = 0;
        diag = diagBase;
        elem = row;
        do {
            if (r == c) {
                *(f32 *)diag = one;
            } else {
                *(s32 *)elem = 0;
            }
            c++;
            elem += 4;
        } while (c < 4);
        diagBase += 0x14;
        r++;
        row += 0x10;
    } while (r < 4);
    outLo = arg0;
    outHi = arg0 + 8;
    i = 0;
    mult = D_800AA7C4;
    mask = 0xFFFF0000;
    inRow = matrix;
    do {
        j = 0;
        inPair = inRow;
        do {
            t1 = inPair[0] * mult;
            t0 = inPair[1] * mult;
            inPair += 2;
            j++;
            v0 = (s32)t1;
            v1 = (s32)t0;
            *outLo = (v0 & mask) | ((u32)v1 >> 16);
            outLo++;
            *outHi = ((v0 << 16) & mask) | (v1 & 0xFFFF);
            outHi++;
        } while (j < 2);
        i++;
        inRow += 4;
    } while (i < 4);
}
