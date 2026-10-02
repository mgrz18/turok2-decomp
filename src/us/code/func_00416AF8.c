#include "common.h"
#include "m2c_macros.h"

s32 func_00416AF8(void **arg0) {
    s32 var_v1;
    void *var_a0;

    var_a0 = *arg0;
    var_v1 = 0;
    if (var_a0 != NULL) {
        do {
            if (M2C_FIELD(var_a0, s16 *, 0x28) != 4) {
                var_v1 += 1;
            }
            var_a0 = M2C_FIELD(var_a0, void **, 0x1D4);
        } while (var_a0 != NULL);
    }
    return var_v1;
}
