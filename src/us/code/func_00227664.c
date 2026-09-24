#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800F1360;

void func_00227664(void *arg0) {
    M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800F1360;
    M2C_FIELD(arg0, s8 *, 0) = 3;
    M2C_FIELD(arg0, s32 *, 0x1A0) = 0;
}
