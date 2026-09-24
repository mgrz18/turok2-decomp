#include "common.h"
#include "m2c_macros.h"

void func_00254B6C(void *arg0) {
    void *temp_a0;
    void *var_v1;

    var_v1 = M2C_FIELD(arg0, void **, 0x20);
    if (var_v1 != NULL) {
        do {
            temp_a0 = M2C_FIELD(var_v1, void **, 0x518);
            if (M2C_FIELD(temp_a0, u8 *, 0x3F) == 1) {
                M2C_FIELD(temp_a0, u8 *, 0x3F) = 0U;
            }
            var_v1 = M2C_FIELD(var_v1, void **, 0x1320);
        } while (var_v1 != NULL);
    }
}
