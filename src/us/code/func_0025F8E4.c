#include "common.h"
#include "m2c_macros.h"

void func_0025F8E4(s32 arg0, void *arg1) {
    s32 v = M2C_FIELD(arg1, u8 *, 0xC8);

    v += 0x19;
    if (v >= 0x100) {
        v = 0xFF;
    }
    M2C_FIELD(arg1, u8 *, 0xC8) = v;
}
