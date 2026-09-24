#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800F1850;

void func_002317C0(void *arg0) {
    M2C_FIELD(arg0, M2C_UNK **, 0x14) = &D_800F1850;
    M2C_FIELD(arg0, s8 *, 0) = 2;
    M2C_FIELD(arg0, s32 *, 0x40) = 0;
    M2C_FIELD(arg0, s32 *, 0x1B8) = 0;
    M2C_FIELD(arg0, s32 *, 0x1C0) = 0;
}
