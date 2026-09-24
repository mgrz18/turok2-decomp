#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00264C8C(s32, s32, s32);               /* extern */

void func_0025E488(s32 arg0, void *arg1, s32 arg2) {
    M2C_FIELD(arg1, s32 *, 0x114) = arg2;
    if (arg2 != 0) {
        func_00264C8C(arg2, arg2 + 0x140, arg0);
    }
}
