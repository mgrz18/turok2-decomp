#include "common.h"
#include "m2c_macros.h"

void func_0025FAB0(s32 arg0, void *arg1) {
    s32 v = M2C_FIELD(arg1, u8 *, 0xC8);

    v += 0xA;
    if (v >= 0x100) {
        v = 0xFF;
    }
    M2C_FIELD(arg1, u8 *, 0xC8) = v;
    M2C_FIELD(arg1, s32 *, 0x64) = 0;
}
