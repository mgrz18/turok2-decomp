/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8024BA7C_de_15.c, func_8024CC30_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A5D58;
extern float D_800A5D50;
extern float D_800A5D30;
extern void func_0021FD30(f32 *src, PackedMatrixWords *dst);
extern float D_800A5D48;
extern float D_800A5D40;
extern float D_800A5D38;
void func_0021FD30(f32 *src, PackedMatrixWords *dst) {
    u32 *upper = dst->upper;
    u32 *lower = dst->lower;
    s32 a;
    s32 b;
    { f32 value = (src[0]); if (!((D_800A5D30) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (D_800A5D30)); (a) |= 0x80000000; } };
    { f32 value = (src[1]); if (!((*(&D_800A5D30 + 1)) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (*(&D_800A5D30 + 1))); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[2]); if (!((D_800A5D38) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (D_800A5D38)); (a) |= 0x80000000; } };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    { f32 value = (src[4]); if (!((*(&D_800A5D38 + 1)) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (*(&D_800A5D38 + 1))); (a) |= 0x80000000; } };
    { f32 value = (src[5]); if (!((D_800A5D40) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (D_800A5D40)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[6]); if (!((*(&D_800A5D40 + 1)) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (*(&D_800A5D40 + 1))); (a) |= 0x80000000; } };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    { f32 value = (src[8]); if (!((D_800A5D48) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (D_800A5D48)); (a) |= 0x80000000; } };
    { f32 value = (src[9]); if (!((*(&D_800A5D48 + 1)) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (*(&D_800A5D48 + 1))); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[10]); if (!((D_800A5D50) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (D_800A5D50)); (a) |= 0x80000000; } };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    { f32 value = (src[12]); if (!((*(&D_800A5D50 + 1)) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (*(&D_800A5D50 + 1))); (a) |= 0x80000000; } };
    { f32 value = (src[13]); if (!((D_800A5D58) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - (D_800A5D58)); (b) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    { f32 value = (src[14]); if (!((*(&D_800A5D58 + 1)) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - (*(&D_800A5D58 + 1))); (a) |= 0x80000000; } };
    *upper = (a & 0xFFFF0000) | 1;
    *lower = a << 16;
}
