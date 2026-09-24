#include "common.h"
#include "m2c_macros.h"

s32 func_0021A7EC(s32);
s32 func_0021E9A4(s32, s32, s32);

void func_002666B0(s32 arg0, void *arg1, s32 arg2) {
    M2C_FIELD(arg1, s8 *, 0x35) = -1;
    M2C_FIELD(arg1, s8 *, 0xC7) = 0;
    M2C_FIELD(arg1, s32 *, 0x10C) = arg2;
    func_0021E9A4(arg0, arg2, 1);
    func_0021A7EC(arg0);
}
