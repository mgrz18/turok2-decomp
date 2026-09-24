#include "common.h"
#include "m2c_macros.h"

s32 func_0021A7EC(s32);
s32 func_0021E9A4(s32, s32, s32);
s32 func_00245A98(s32, s32, s32, s32);

extern M2C_UNK D_800AF6F0;

void func_00266854(s32 arg0, void *arg1) {
    s32 temp_a1;

    if (((u32) (M2C_FIELD(arg1, u16 *, 0xC4) - 0xFA0) < 0x401U) && (M2C_FIELD(arg1, s8 *, 0xC7) != 0)) {
        temp_a1 = func_00245A98(arg0, (s32) arg1, (s32) &D_800AF6F0, -1);
        M2C_FIELD(arg1, s8 *, 0x35) = -1;
        M2C_FIELD(arg1, s8 *, 0xC7) = 0;
        M2C_FIELD(arg1, s32 *, 0x10C) = temp_a1;
        func_0021E9A4(arg0, temp_a1, 1);
        func_0021A7EC(arg0);
    }
}
