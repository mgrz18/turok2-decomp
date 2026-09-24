#include "common.h"
#include "m2c_macros.h"

f32 func_0020F6BC(M2C_UNK, void *);                 /* extern */

f32 func_00256CE0(void *arg0, M2C_UNK arg1) {
    f32 var_f20;
    void *var_s0;

    var_s0 = M2C_FIELD(arg0, void **, 0x20);
    var_f20 = 0.0f;
    if (var_s0 != NULL) {
        do {
            if (M2C_FIELD(var_s0, s32 *, 0x51C) != 0) {
                var_f20 += func_0020F6BC(arg1, var_s0 + 4);
            }
            var_s0 = M2C_FIELD(var_s0, void **, 0x1320);
        } while (var_s0 != NULL);
    }
    return var_f20;
}
