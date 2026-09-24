#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00275864(s32);                         /* extern */
extern s32 D_800B2A88;

void func_00275DC0(void *arg0) {
    s32 temp_v0;
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x14);
    if (var_s0 != NULL) {
        do {
            func_00275864(M2C_FIELD(var_s0, s32 *, 8));
            temp_v0 = M2C_FIELD(var_s0, s32 *, 8);
            var_s0 = M2C_FIELD(var_s0, void **, 4);
            D_800B2A88 = temp_v0;
        } while (var_s0 != NULL);
    }
    M2C_FIELD(arg0, s32 *, 0x28) = 1;
}
