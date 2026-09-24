#include "common.h"
#include "m2c_macros.h"

extern void *D_800C1BB0;

s32 func_00284188(void) {
    f32 temp_f1;
    s32 var_a0;

    var_a0 = 0;
    if (M2C_FIELD(D_800C1BB0, s32 *, 0x38) != 0) {
        temp_f1 = M2C_FIELD(D_800C1BB0, f32 *, 0x1C);
        if ((M2C_FIELD(D_800C1BB0, f32 *, 0x30) <= temp_f1) && (temp_f1 <= M2C_FIELD(D_800C1BB0, f32 *, 0x34))) {
            var_a0 = 1;
        }
    }
    return var_a0;
}
