#include "common.h"
#include "m2c_macros.h"

extern u8 D_00296360[];
extern s32 func_002964A0(s32, s32, s32);
extern void func_00296340(void *, void *, void *, s32);

void func_002953B4(void *arg0, s32 arg1, s32 arg2) {
    func_00296340(arg0, D_00296360, func_002964A0, 7);
    M2C_FIELD(arg0, s32 *, 0x14) = 0;
    M2C_FIELD(arg0, s32 *, 0x18) = arg2;
    M2C_FIELD(arg0, s32 *, 0x1C) = arg1;
}
