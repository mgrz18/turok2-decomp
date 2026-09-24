#include "common.h"
#include "m2c_macros.h"

s32 func_00416AF8(s32);                             /* extern */

s32 func_00254BE4(void *arg0) {
    s32 var_s1;
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x20);
    var_s1 = 0;
    if (var_s0 != NULL) {
        do {
            if ((M2C_FIELD(var_s0, s32 *, 0x510) != 0) && (func_00416AF8(M2C_FIELD(var_s0, s32 *, 0x51C) + 0x53C) == 0)) {
                var_s1 += 1;
            }
            var_s0 = M2C_FIELD(var_s0, void **, 0x1320);
        } while (var_s0 != NULL);
    }
    return var_s1;
}
