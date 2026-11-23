/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_15.c, func_8024CC30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern void CMtxF__ToMtxAffineUnscaled(f32 *src, PackedMatrixWords *dst);
void CMtxF__ToMtxAffineUnscaled(f32 *src, PackedMatrixWords *dst) {
    u32 *upper = dst->upper;
    u32 *lower = dst->lower;
    s32 a;
    s32 b;
    { f32 value = (src[0]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    { f32 value = (src[1]); if (!((2.1474836e9f) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (2.1474836e9f)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[2]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    { f32 value = (src[4]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    { f32 value = (src[5]); if (!((2.1474836e9f) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (2.1474836e9f)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[6]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    { f32 value = (src[8]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    { f32 value = (src[9]); if (!((2.1474836e9f) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (2.1474836e9f)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[10]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    { f32 value = (src[12]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    { f32 value = (src[13]); if (!((2.1474836e9f) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (2.1474836e9f)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[14]); if (!((2.1474836e9f) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (2.1474836e9f)); (a) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | 1;
    *lower = a << 16;
}
