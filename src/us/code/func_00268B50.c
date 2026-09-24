#include "common.h"
#include "m2c_macros.h"

f32 func_00268B50(void *arg0) {
    f32 temp_f1;
    f32 var_f0;
    f32 var_f1;
    void *temp_a1;

    temp_a1 = M2C_FIELD(arg0, void **, 0x10);
    var_f0 = M2C_FIELD(M2C_FIELD(temp_a1, void **, 8), f32 *, 4);
    temp_f1 = M2C_FIELD(M2C_FIELD(temp_a1, void **, 4), f32 *, 4);
    if (!(temp_f1 <= var_f0)) {
        var_f0 = temp_f1;
    }
    var_f1 = M2C_FIELD(M2C_FIELD(temp_a1, void **, 0xC), f32 *, 4);
    if (!(var_f0 <= var_f1)) {
        var_f1 = var_f0;
    }
    return var_f1 - M2C_FIELD(arg0, f32 *, 8);
}
