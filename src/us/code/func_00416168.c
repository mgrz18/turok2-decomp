#include "common.h"
#include "m2c_macros.h"

s32 func_00413F74(s32, s32, s32, s32, s32);

void func_00416168(void *arg0, s32 arg1, s32 arg2, s32 arg3) {
    func_00413F74((s32) arg0, arg1, arg2, arg3, M2C_FIELD(arg0, s32 *, 0x14));
}
