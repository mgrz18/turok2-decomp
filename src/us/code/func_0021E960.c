#include "common.h"
#include "m2c_macros.h"

void func_0021E960(u8 *arg0, s32 arg1) {
    M2C_FIELD(arg0, s16 *, 0xDE) = arg1;
    M2C_FIELD(arg0, u8 *, 0xE2) = 0;
    if (M2C_FIELD(arg0, s16 *, 0xDC) != arg1) {
        M2C_FIELD(arg0, u8 *, 0xE3) = 1;
    }
}
