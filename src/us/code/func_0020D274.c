/* From AbuN64/ragewars-decomp (Turok: Rage Wars): src/func_8026E1F8_de_6.c, func_8027027C_de.
 * The same code in Turok 2; symbols renamed to Turok 2's by tools/import_ref.py. */

#include "rw/abu_types.h"

extern float D_800A54A4;
extern float D_800A54EC;
extern float D_800A54CC;
extern float D_800A54B4;
extern float D_800A54AC;
extern float D_800A54E4;
extern float D_800A54BC;
extern float D_800A54C4;
extern float D_800A549C;
extern float D_800A5494;
extern float D_800A54D4;
extern float D_800A54DC;
void func_0020D274(f32 *src, PackedMatrixWords *dst) {
    u32 *upper = dst->upper;
    u32 *lower = dst->lower;
    f32 value;
    s32 a;
    s32 b;
    value = (src[0]) * (D_800A5494); if (!(*(&(D_800A5494) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A5494) + 1)); (a) |= 0x80000000; };
    value = (src[1]) * (D_800A549C); if (!(*(&(D_800A549C) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A549C) + 1)); (b) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    value = (src[2]) * (D_800A54A4); if (!(*(&(D_800A54A4) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A54A4) + 1)); (a) |= 0x80000000; };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    value = (src[4]) * (D_800A54AC); if (!(*(&(D_800A54AC) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A54AC) + 1)); (a) |= 0x80000000; };
    value = (src[5]) * (D_800A54B4); if (!(*(&(D_800A54B4) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A54B4) + 1)); (b) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    value = (src[6]) * (D_800A54BC); if (!(*(&(D_800A54BC) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A54BC) + 1)); (a) |= 0x80000000; };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    value = (src[8]) * (D_800A54C4); if (!(*(&(D_800A54C4) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A54C4) + 1)); (a) |= 0x80000000; };
    value = (src[9]) * (D_800A54CC); if (!(*(&(D_800A54CC) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A54CC) + 1)); (b) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    value = (src[10]) * (D_800A54D4); if (!(*(&(D_800A54D4) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A54D4) + 1)); (a) |= 0x80000000; };
    *upper = a & 0xFFFF0000;
    *lower = a << 16;
    upper++;
    lower++;
    value = (src[12]) * (D_800A54DC); if (!(*(&(D_800A54DC) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A54DC) + 1)); (a) |= 0x80000000; };
    value = (src[13]) * (D_800A54E4); if (!(*(&(D_800A54E4) + 1) <= value)) { (b) = (s32)value; } else { (b) = (s32)(value - *(&(D_800A54E4) + 1)); (b) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | ((u32)b >> 16);
    *lower = (a << 16) | (b & 0xFFFF);
    upper++;
    lower++;
    value = (src[14]) * (D_800A54EC); if (!(*(&(D_800A54EC) + 1) <= value)) { (a) = (s32)value; } else { (a) = (s32)(value - *(&(D_800A54EC) + 1)); (a) |= 0x80000000; };
    *upper = (a & 0xFFFF0000) | 1;
    *lower = a << 16;
}
