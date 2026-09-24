#include "common.h"
#include "m2c_macros.h"

s32 func_00275544(s16, s32, s32, s32, s32, s32);    /* extern */
extern s32 D_800B2A8C;

void func_00275E1C(void *arg0) {
    s32 temp_v0;
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x14);
    if (var_s0 != NULL) {
        do {
            temp_v0 = func_00275544(M2C_FIELD(var_s0, s16 *, 0xE), M2C_FIELD(var_s0, s32 *, 0x10), M2C_FIELD(var_s0, s32 *, 0x14), M2C_FIELD(var_s0, s32 *, 0x18), M2C_FIELD(var_s0, s32 *, 0x1C), -1);
            M2C_FIELD(var_s0, s32 *, 8) = temp_v0;
            var_s0 = M2C_FIELD(var_s0, void **, 4);
            D_800B2A8C = temp_v0;
        } while (var_s0 != NULL);
    }
    M2C_FIELD(arg0, s32 *, 0x28) = 0;
}
