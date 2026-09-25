#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_800B6CB0;

void func_00280E64(void *arg0, s16 arg1) {
    M2C_UNK (*temp_v0)();

    M2C_FIELD(arg0, s16 *, 0x14) = arg1;
    M2C_FIELD(arg0, s32 *, 0x18) = 0;
    M2C_FIELD(arg0, s16 *, 0x1C) = 0;
    M2C_FIELD(arg0, s32 *, 0x20) = 0;
    temp_v0 = *(((__typeof__(&D_800B6CB0))((s8 *)&D_800B6CB0 + (M2C_FIELD(arg0, s16 *, 0x14) * 8))));
    if (temp_v0 != NULL) {
        temp_v0();
    }
}
