#include "common.h"
#include "m2c_macros.h"

u32 func_002912A0(s32 arg0, s32 arg1, void *arg2, s32 arg3, s32 arg4) {
    u32 temp_a1;
    u32 temp_v1;
    u32 var_a3;

    temp_a1 = M2C_FIELD(arg2, u32 *, 4);
    temp_v1 = temp_a1 + (((arg3 * arg4) + 0xF) & ~0xF);
    var_a3 = 0;
    if ((u32) (M2C_FIELD(arg2, s32 *, 0) + M2C_FIELD(arg2, s32 *, 8)) >= temp_v1) {
        var_a3 = temp_a1;
        M2C_FIELD(arg2, u32 *, 4) = temp_v1;
    }
    return var_a3;
}
