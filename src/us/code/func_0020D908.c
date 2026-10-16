/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_80270910_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
extern float D_800A54FC;
extern float D_800A54F8;
struct PackedMatrixWords;
struct PackedMatrixWords {
    u32 upper[8];
    u32 lower[8];
};
struct PackedMatrixWords;
typedef struct PackedMatrixWords PackedMatrixWords;
void func_0020D908(f32 *dst, PackedMatrixWords *src) {
    u32 upper;
    u32 lower;
    f32 scale = D_800A54F8;
    upper = src->upper[0]; lower = src->lower[0]; dst[(0) * 2] = (f32)(s32)((upper & 0xFFFF0000) | (lower >> 16)) * scale; dst[(0) * 2 + 1] = (f32)(s32)((upper << 16) | (lower & 0xFFFF)) * scale;
    upper = src->upper[1]; lower = src->lower[1]; dst[(1) * 2] = (f32)(s32)(upper | (lower >> 16)) * scale; dst[(1) * 2 + 1] = 0.0f;
    upper = src->upper[2]; lower = src->lower[2]; dst[(2) * 2] = (f32)(s32)((upper & 0xFFFF0000) | (lower >> 16)) * scale; dst[(2) * 2 + 1] = (f32)(s32)((upper << 16) | (lower & 0xFFFF)) * scale;
    upper = src->upper[3]; lower = src->lower[3]; dst[(3) * 2] = (f32)(s32)(upper | (lower >> 16)) * scale; dst[(3) * 2 + 1] = 0.0f;
    upper = src->upper[4]; lower = src->lower[4]; dst[(4) * 2] = (f32)(s32)((upper & 0xFFFF0000) | (lower >> 16)) * scale; dst[(4) * 2 + 1] = (f32)(s32)((upper << 16) | (lower & 0xFFFF)) * scale;
    upper = src->upper[5]; lower = src->lower[5]; dst[(5) * 2] = (f32)(s32)(upper | (lower >> 16)) * scale; dst[(5) * 2 + 1] = 0.0f;
    upper = src->upper[6]; lower = src->lower[6]; dst[(6) * 2] = (f32)(s32)((upper & 0xFFFF0000) | (lower >> 16)) * scale; dst[(6) * 2 + 1] = (f32)(s32)((upper << 16) | (lower & 0xFFFF)) * scale;
    {
        f32 last;
        upper = src->upper[7];
        lower = src->lower[7];
        last = (f32)(s32)((upper & 0xFFFF0000) | (lower >> 16)) * scale;
        dst[15] = D_800A54FC;
        dst[14] = last;
    }
}
