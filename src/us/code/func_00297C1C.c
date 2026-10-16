/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_802B53FC_de_padding_B5FFC_21.c, func_802B6CBC_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern float D_800AA7CC;
void func_00297C1C(f32 *arg0, s32 *arg1) {
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
    outLo = arg1;
    outHi = arg1 + 8;
    i = 0;
    mult = D_800AA7CC;
    mask = 0xFFFF0000;
    inRow = arg0;
    do {
        j = 0;
        inPair = inRow;
        do {
            t1 = inPair[0] * mult;
            t0 = inPair[1] * mult;
            inPair += 2;
            j += 1;
            v0 = (s32) t1;
            v1 = (s32) t0;
            *outLo = (v0 & mask) | ((u32) v1 >> 16);
            outLo += 1;
            *outHi = ((v0 << 16) & mask) | (v1 & 0xFFFF);
            outHi += 1;
        } while (j < 2);
        i += 1;
        inRow += 4;
    } while (i < 4);
}
