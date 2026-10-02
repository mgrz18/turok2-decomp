#include "common.h"
#include "m2c_macros.h"

void func_00416C28(void *arg0) {
    s32 temp_v0;

    temp_v0 = M2C_FIELD(arg0, s32 *, 0x1C8) + M2C_FIELD(arg0, s32 *, 0x1CC);
    M2C_FIELD(arg0, s32 *, 0x1C8) = temp_v0;
    if (temp_v0 >= 0x30) {
        M2C_FIELD(arg0, s32 *, 0x1C8) = 0;
        M2C_FIELD(arg0, s32 *, 0x1CC) = 0;
    }
}
