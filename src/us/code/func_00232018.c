#include "common.h"
#include "m2c_macros.h"

void func_00232018(void *arg0, s32 arg1) {
    if ((M2C_FIELD(arg0, u16 *, 2) == 0x56) && (arg1 == 2)) {
        M2C_FIELD(arg0, s32 *, 0x40) = (s32) (M2C_FIELD(arg0, s32 *, 0x40) | 0x04000000);
    }
}
