#include "common.h"
#include "m2c_macros.h"

extern f32 D_800AA410;

void func_002929A0(void *arg0, s32 arg1) {
    s32 temp_a1;

    temp_a1 = arg1 * 0x10;
    M2C_FIELD((temp_a1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 6) = 0;
    M2C_FIELD((temp_a1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 0xA) = 0;
    M2C_FIELD((temp_a1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 7) = 0x40;
    M2C_FIELD((temp_a1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 9) = 0x7F;
    M2C_FIELD((temp_a1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 8) = 5;
    M2C_FIELD((temp_a1 + M2C_FIELD(arg0, s32 *, 0x60)), s8 *, 0xB) = 0;
    M2C_FIELD((temp_a1 + M2C_FIELD(arg0, s32 *, 0x60)), s16 *, 4) = 0xC8;
    M2C_FIELD((temp_a1 + M2C_FIELD(arg0, s32 *, 0x60)), f32 *, 0xC) = (f32) D_800AA410;
}
