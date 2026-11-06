/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_8027027C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void CMtxF__ToMtxAffine(f32 *src, PackedMatrixWords *dst) {
    u32 *upper = dst->upper;
    u32 *lower = dst->lower;
    f32 value;
    s32 a;
    s32 b;
    value = (src[0]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; };
    value = (src[1]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    value = (src[2]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    value = (src[4]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; };
    value = (src[5]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    value = (src[6]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    value = (src[8]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; };
    value = (src[9]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    value = (src[10]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    value = (src[12]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; };
    value = (src[13]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    value = (src[14]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | 1;
    *lower = a << 16;
}
