/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_80272FBC_de.c, func_80273E64_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef float f32;
extern void func_00210D34(f32 *dst, f32 *src);
extern const f32 D_800A562C;
extern f32 D_800A562C;
void func_00210D34(f32 *dst, f32 *src) {
    dst[0] = src[0];
    dst[1] = src[2];
    dst[2] = src[1];
    dst[4] = src[4];
    dst[5] = src[6];
    dst[6] = src[5];
    dst[8] = src[8];
    dst[9] = src[10];
    dst[10] = src[9];
    dst[12] = src[12];
    dst[13] = src[14];
    dst[14] = src[13];
    dst[3] = dst[7] = dst[11] = 0.0f;
    dst[15] = D_800A562C;
}
