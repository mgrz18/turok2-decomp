#include "common.h"
#include "m2c_macros.h"

void func_00200500(s32, s32, s32);

void func_0044F614(s32 arg0) {
    func_00200500(arg0, 0x1D0, 0x1D4);
    M2C_FIELD(arg0, s16 *, 0x14) = 0;
}
