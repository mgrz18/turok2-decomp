#include "common.h"
#include "m2c_macros.h"

s32 func_00413230(s32);
s32 func_00413F74(s32, s32, s32, s32, s32);

void func_004161B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_00413F74(arg0, arg1, arg2, arg3, func_00413230(arg0));
}
