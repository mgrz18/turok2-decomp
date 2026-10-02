#include "common.h"
#include "m2c_macros.h"

void func_00416C0C(void *arg0) {
    if (M2C_FIELD(arg0, s32 *, 0x1CC) == 0) {
        M2C_FIELD(arg0, s32 *, 0x1CC) = 1;
        M2C_FIELD(arg0, s32 *, 0x1C8) = 0;
    }
}
