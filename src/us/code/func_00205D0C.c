#include "common.h"
#include "m2c_macros.h"

s32 func_0020185C(s32, s32, s32, s32);
s32 func_0020367C(s32, s32, s32, s32, s32, s32, s32, s32);

void func_00205D0C(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6) {
    s32 sp20;

    func_0020367C(0, func_0020185C(arg1, arg3, arg2, (s32) &sp20), sp20, 0x1B, arg4, arg5, arg6, 1);
}
