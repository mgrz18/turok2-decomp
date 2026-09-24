#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00207914(void *, s32);                 /* extern */

void func_0026D070(void *arg0, void *arg1) {
    s32 temp_a1;
    s32 var_v0;

    M2C_FIELD(arg0, s32 *, 0) = (s32) M2C_FIELD(arg1, s32 *, 8);
    M2C_FIELD(arg0, u8 *, 0x10) = (u8) M2C_FIELD(arg1, u8 *, 0xC);
    M2C_FIELD(arg0, u8 *, 0x11) = (u8) M2C_FIELD(arg1, u8 *, 0xD);
    M2C_FIELD(arg0, u8 *, 0x12) = (u8) M2C_FIELD(arg1, u8 *, 0xE);
    M2C_FIELD(arg0, u8 *, 0x13) = (u8) M2C_FIELD(arg1, u8 *, 0xF);
    M2C_FIELD(arg0, u8 *, 0x14) = (u8) M2C_FIELD(arg1, u8 *, 0x10);
    M2C_FIELD(arg0, u8 *, 0x15) = (u8) M2C_FIELD(arg1, u8 *, 0x11);
    M2C_FIELD(arg0, u8 *, 0x16) = (u8) M2C_FIELD(arg1, u8 *, 0x12);
    M2C_FIELD(arg0, u8 *, 0x17) = (u8) M2C_FIELD(arg1, u8 *, 0x13);
    M2C_FIELD(arg0, u16 *, 0x18) = (u16) M2C_FIELD(arg1, u16 *, 0x14);
    M2C_FIELD(arg0, u16 *, 0x1A) = (u16) M2C_FIELD(arg1, u16 *, 0x16);
    if (M2C_FIELD(arg1, s32 *, 8) & 0x40) {
        var_v0 = (M2C_FIELD(arg1, s32 *, 4) * 2) | 1;
    } else {
        var_v0 = M2C_FIELD(arg1, s32 *, 4) * 2;
    }
    M2C_FIELD(arg0, s32 *, 4) = var_v0;
    temp_a1 = M2C_FIELD(arg1, s32 *, 0);
    if (temp_a1 == -1) {
        M2C_FIELD(arg0, s32 *, 8) = 0;
        return;
    }
    func_00207914(arg0 + 8, temp_a1);
}
