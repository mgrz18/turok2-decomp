#include "common.h"
#include "m2c_macros.h"

void func_002535E4(void *arg0, f32 arg1) {
    void *var_v0;

    var_v0 = M2C_FIELD(arg0, void **, 0x20);
    if (var_v0 != NULL) {
        do {
            M2C_FIELD(var_v0, f32 *, 0xA28) = arg1;
            var_v0 = M2C_FIELD(var_v0, void **, 0x1320);
        } while (var_v0 != NULL);
    }
}
