/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_8026FC9C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

void CMtxF__ToMtx(f32 *src, PackedMatrixWords *dst) {
    u32 *upper = dst->upper;
    u32 *lower = dst->lower;
    f32 value;
    s32 a;
    s32 b;
    value = (src[(0) * 2]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; }; value = (src[(0) * 2 + 1]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(1) * 2]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; }; value = (src[(1) * 2 + 1]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(2) * 2]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; }; value = (src[(2) * 2 + 1]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(3) * 2]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; }; value = (src[(3) * 2 + 1]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(4) * 2]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; }; value = (src[(4) * 2 + 1]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(5) * 2]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; }; value = (src[(5) * 2 + 1]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(6) * 2]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; }; value = (src[(6) * 2 + 1]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
    value = (src[(7) * 2]) * (65536.0f); if (!(2.1474836e9f <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - 2.1474836e9f); (a) |= 0x80000000; }; value = (src[(7) * 2 + 1]) * (65536.0f); if (!(2.1474836e9f <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - 2.1474836e9f); (b) |= 0x80000000; }; *upper = (a & 0xFFFF0000) | ((u32)b >> 16); *lower = (a << 16) | (b & 0xFFFF); upper++; lower++;
}
