#include "common.h"
#include "m2c_macros.h"

void func_0042F838(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0xC) - 1;
    M2C_FIELD(arg0, s32 *, 0xC) = temp_v0;
    if (temp_v0 <= 0) {
        M2C_FIELD(arg0, M2C_UNK (**)(), 0x14)();
    }
}
