#include "common.h"
#include "m2c_macros.h"

void func_00292A20(void *arg0, void *arg1, s32 arg2) {
    s32 temp_a2;

    temp_a2 = arg2 * 0x10;
    *(s32 *)(temp_a2 + M2C_FIELD(arg0, s32 *, 0x60)) = arg1;
    M2C_FIELD((temp_a2 + M2C_FIELD(arg0, s32 *, 0x60)), u8 *, 7) = M2C_FIELD(arg1, u8 *, 1);
    M2C_FIELD((temp_a2 + M2C_FIELD(arg0, s32 *, 0x60)), u8 *, 9) = M2C_FIELD(arg1, u8 *, 0);
    M2C_FIELD((temp_a2 + M2C_FIELD(arg0, s32 *, 0x60)), u8 *, 8) = M2C_FIELD(arg1, u8 *, 2);
    M2C_FIELD((temp_a2 + M2C_FIELD(arg0, s32 *, 0x60)), u16 *, 4) = M2C_FIELD(arg1, u16 *, 0xC);
}
