#include "common.h"
#include "m2c_macros.h"

extern s32 func_00294D54(s32, s32, s32, s32, s32);
extern s32 func_00294E3C(s32, s32, s32);
extern void func_00296340(void *, void *, void *, s32);

void func_00295410(void *arg0, s32 arg1, s32 arg2) {
    func_00296340(arg0, (s8 *)func_00294D54 + 0xC, func_00294E3C, 6);
    M2C_FIELD(arg0, s32 *, 0x14) = 0;
    M2C_FIELD(arg0, s32 *, 0x18) = arg2;
    M2C_FIELD(arg0, s32 *, 0x1C) = arg1;
}
