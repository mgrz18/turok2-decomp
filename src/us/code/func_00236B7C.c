#include "common.h"
#include "m2c_macros.h"

void func_00236B7C(void *arg0) {
    f32 temp_f0;
    f32 var_f1;
    void *var_v0;

    if (M2C_FIELD(arg0, f32 *, 0x4C) > 0.0f) {
        var_f1 = 0.0f;
        var_v0 = M2C_FIELD(arg0, void **, 0x40);
        if (var_v0 != NULL) {
            do {
                M2C_FIELD(var_v0, f32 *, 0xA8) = (f32) (var_f1 / M2C_FIELD(arg0, f32 *, 0x4C));
                temp_f0 = M2C_FIELD(var_v0, f32 *, 0xAC);
                var_v0 = M2C_FIELD(var_v0, void **, 4);
                var_f1 += temp_f0;
            } while (var_v0 != NULL);
        }
    }
}
