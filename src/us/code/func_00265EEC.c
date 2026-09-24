#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

void func_00265EEC(void *arg0, void *arg1, void *arg2) {
    s32 temp_v1;
    s32 var_a2;
    s32 var_a3;
    s32 var_v1;

    temp_v1 = M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0x14);
    var_a3 = 0;
    if (temp_v1 & 1) {
        var_a3 = (((u32) M2C_FIELD(arg2, u32 *, 0xC) >> 0x16) ^ 1) & 1;
    }
    if ((temp_v1 & 2) && (M2C_FIELD(arg2, u32 *, 0xC) & 0x400000)) {
        var_a3 = 1;
    }
    if (var_a3 != 0) {
        var_v1 = M2C_FIELD(arg1, s32 *, 4) - M2C_FIELD(arg2, s32 *, 4);
        if (var_v1 < 0) {
            var_v1 = 0;
        }
        M2C_FIELD(arg1, s32 *, 4) = var_v1;
        if (var_v1 == 0) {
            var_a2 = 1;
            if (M2C_FIELD(M2C_FIELD(arg0, void **, 0x14), s32 *, 0x14) & 0x40) {
                var_a2 = 2;
            }
            func_00243414((s32) arg0, (s32) arg1, var_a2);
        }
    }
}
