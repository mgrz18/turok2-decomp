#include "common.h"
#include "m2c_macros.h"

s32 func_00254B9C(void *arg0) {
    s32 var_a0;
    void *temp_v0;
    void *var_v1;

    var_v1 = M2C_FIELD(arg0, void **, 0x20);
    var_a0 = 0;
    if (var_v1 != NULL) {
        do {
            temp_v0 = M2C_FIELD(var_v1, void **, 0x51C);
            if ((temp_v0 != NULL) && (M2C_FIELD(temp_v0, s32 *, 0x54C) == 0) && (M2C_FIELD(M2C_FIELD(var_v1, void **, 0x518), u8 *, 0x3F) == 0)) {
                var_a0 += 1;
            }
            var_v1 = M2C_FIELD(var_v1, void **, 0x1320);
        } while (var_v1 != NULL);
    }
    return var_a0;
}
