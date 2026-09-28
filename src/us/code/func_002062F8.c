#include "common.h"
#include "m2c_macros.h"

s32 func_00200738(s32, s32);

extern s32 D_800D81D4;
extern M2C_UNK D_800D81E0;
extern s32 D_800D8DAC;

void func_002062F8(s32 arg0, s32 arg1) {
    func_00200738((s32) &D_800D81E0, arg1);
    if (M2C_FIELD(arg1, s32 *, 0xC) & 0x1000) {
        func_00200738((s32) (((__typeof__(&D_800D81E0))((s8 *)&D_800D81E0 + 0x14))), arg1);
    }
    if (M2C_FIELD(&D_800D81E0, s32 *, -8) == arg1) {
        M2C_FIELD(&D_800D81E0, s32 *, -8) = 0;
    }
    M2C_FIELD(arg1, s32 *, 0xC) = 0;
    *(s32 *)((D_800D8DAC * 4) + D_800D81D4) = arg1;
    M2C_FIELD(&D_800D81E0, s32 *, 0xBCC) = (s32) (M2C_FIELD(&D_800D81E0, s32 *, 0xBCC) + 1);
}
