#include "common.h"
#include "m2c_macros.h"

s32 func_00266644(s32, s32);

void func_00266490(s32 arg0, void *arg1) {
    M2C_FIELD(arg1, s32 (**)(s32, s32), 0xF4) = func_00266644;
}
