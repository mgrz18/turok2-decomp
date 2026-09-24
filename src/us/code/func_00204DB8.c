#include "common.h"
#include "m2c_macros.h"

void func_00204DB8(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 8) - 1;
    M2C_FIELD(arg0, s32 *, 8) = temp_v0;
    if (temp_v0 == 0) {
        M2C_FIELD(arg0, s32 *, 0xC) = (s32) (M2C_FIELD(arg0, s32 *, 0xC) & ~0x100);
    }
}
