#include "common.h"
#include "m2c_macros.h"

void func_00253624(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0xB4);
    if ((temp_v0 != 0) && (temp_v0 != 3)) {
        M2C_FIELD(arg0, s32 *, 0xB4) = 3;
    }
}
