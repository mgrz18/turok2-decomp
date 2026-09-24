#include "common.h"
#include "m2c_macros.h"

void func_0020280C(void *arg0, s32 *arg1, u32 *arg2) {
    u32 temp_a0;
    u32 var_v1;
    void *var_a3;

    *arg1 = 0;
    *arg2 = 0;
    var_a3 = M2C_FIELD(arg0, void **, 8);
    if (var_a3 != NULL) {
        do {
            *arg1 += M2C_FIELD(var_a3, u32 *, 0x14);
            var_v1 = M2C_FIELD(var_a3, u32 *, 0x14);
            temp_a0 = *arg2;
            if (var_v1 < temp_a0) {
                var_v1 = temp_a0;
            }
            *arg2 = var_v1;
            var_a3 = M2C_FIELD(var_a3, void **, 4);
        } while (var_a3 != NULL);
    }
}
