#include "common.h"
#include "m2c_macros.h"

f32 func_00232ECC(void *arg0) {
    f32 var_f0;

    var_f0 = 0.0f;
    if (M2C_FIELD(M2C_FIELD(arg0, void **, 0xF8), s32 *, 0x14) == 0) {
        var_f0 = M2C_FIELD(arg0, f32 *, 0x160);
    }
    return var_f0;
}
