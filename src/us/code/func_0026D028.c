#include "common.h"
#include "m2c_macros.h"

f32 func_0026D028(void *arg0) {
    f32 temp_f0;
    f32 var_f0;
    f32 var_f1;

    var_f1 = M2C_FIELD(M2C_FIELD(arg0, void **, 8), f32 *, 4);
    temp_f0 = M2C_FIELD(M2C_FIELD(arg0, void **, 4), f32 *, 4);
    if (!(temp_f0 <= var_f1)) {
        var_f1 = temp_f0;
    }
    var_f0 = M2C_FIELD(M2C_FIELD(arg0, void **, 0xC), f32 *, 4);
    if (!(var_f1 <= var_f0)) {
        var_f0 = var_f1;
    }
    return var_f0;
}
