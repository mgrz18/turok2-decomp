#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

extern f32 D_800B6D28;

void func_00264740(void *arg0, void *arg1) {
    s32 temp_a3;
    s32 var_v1;
    void *temp_t0;

    temp_t0 = M2C_FIELD(arg0, s32 *, 0x14) + 0x14;
    temp_a3 = M2C_FIELD(temp_t0, s32 *, 0x24);
    var_v1 = 1;
    if (temp_a3 & 0x20000) {
        var_v1 = (u32)(M2C_FIELD(arg1, s32 *, 0) & 0x20000) < (u32)var_v1;
    }
    if ((temp_a3 & 0x8000) && !(M2C_FIELD(arg1, s32 *, 0) & 0x20000)) {
        var_v1 = 0;
    }
    if (var_v1 != 0) {
        M2C_FIELD(arg1, f32 *, 0x64) = M2C_FIELD(arg1, f32 *, 0x64) - (D_800B6D28 / M2C_FIELD(temp_t0, f32 *, 0x38));
    }
    if (M2C_FIELD(arg1, f32 *, 0x64) <= 0.0f) {
        func_00243414(arg0, arg1, 0);
    }
}
