#include "common.h"
#include "m2c_macros.h"

void func_00200818(void *arg0, s32 arg1) {
    s32 temp_a2;
    s32 temp_a2_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v1;

    temp_a2 = *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 8));
    if (temp_a2 != 0) {
        temp_v0 = M2C_FIELD(arg0, s32 *, 0xC);
        *(s32 *)(temp_a2 + temp_v0) = *(s32 *)(arg1 + temp_v0);
    }
    temp_a2_2 = *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 0xC));
    if (temp_a2_2 != 0) {
        temp_v0_2 = M2C_FIELD(arg0, s32 *, 8);
        *(s32 *)(temp_a2_2 + temp_v0_2) = *(s32 *)(arg1 + temp_v0_2);
    }
    if (M2C_FIELD(arg0, s32 *, 0) == arg1) {
        M2C_FIELD(arg0, s32 *, 0) = (s32) *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 0xC));
    }
    if (M2C_FIELD(arg0, s32 *, 4) == arg1) {
        M2C_FIELD(arg0, s32 *, 4) = (s32) *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 8));
    }
    temp_v1 = M2C_FIELD(arg0, s32 *, 0);
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) - 1);
    if (temp_v1 != 0) {
        *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 0xC)) = temp_v1;
        *(s32 *)(M2C_FIELD(arg0, s32 *, 0) + M2C_FIELD(arg0, s32 *, 8)) = arg1;
    } else {
        *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 0xC)) = 0;
        M2C_FIELD(arg0, s32 *, 4) = arg1;
    }
    *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 8)) = 0;
    M2C_FIELD(arg0, s32 *, 0) = arg1;
    M2C_FIELD(arg0, s32 *, 0x10) = (s32) (M2C_FIELD(arg0, s32 *, 0x10) + 1);
}
