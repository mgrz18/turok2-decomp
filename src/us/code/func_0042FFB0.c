#include "common.h"
#include "m2c_macros.h"

s32 func_00200518(s32, s32);
s32 func_00200738(s32, s32);

void func_0042FFB0(s32 arg0, s32 arg1) {
    func_00200738(arg0 + 0x20, arg1);
    func_00200518(arg0 + 0xC, arg1);
}
