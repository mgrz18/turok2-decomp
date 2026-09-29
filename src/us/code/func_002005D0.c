#include "common.h"
#include "m2c_macros.h"

void func_002005D0(void *arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 var_v0;

    temp_v1 = *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 8));
    if (temp_v1 != 0) {
        *(s32 *)(temp_v1 + M2C_FIELD(arg0, s32 *, 0xC)) = arg2;
        temp_v0 = M2C_FIELD(arg0, s32 *, 8);
        *(s32 *)(arg2 + temp_v0) = *(s32 *)(arg1 + temp_v0);
        *(s32 *)(arg1 + M2C_FIELD(arg0, s32 *, 8)) = arg2;
        *(s32 *)(arg2 + M2C_FIELD(arg0, s32 *, 0xC)) = arg1;
        var_v0 = M2C_FIELD(arg0, s32 *, 0x10) + 1;
    } else {
        temp_v1_2 = M2C_FIELD(arg0, s32 *, 0);
        if (temp_v1_2 != 0) {
            *(s32 *)(arg2 + M2C_FIELD(arg0, s32 *, 0xC)) = temp_v1_2;
            *(s32 *)(M2C_FIELD(arg0, s32 *, 0) + M2C_FIELD(arg0, s32 *, 8)) = arg2;
        } else {
            *(s32 *)(arg2 + M2C_FIELD(arg0, s32 *, 0xC)) = 0;
            M2C_FIELD(arg0, s32 *, 4) = arg2;
        }
        *(s32 *)(arg2 + M2C_FIELD(arg0, s32 *, 8)) = 0;
        M2C_FIELD(arg0, s32 *, 0) = arg2;
        var_v0 = M2C_FIELD(arg0, s32 *, 0x10) + 1;
    }
    M2C_FIELD(arg0, s32 *, 0x10) = var_v0;
}
