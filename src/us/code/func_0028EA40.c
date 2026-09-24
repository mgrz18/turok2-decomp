#include "common.h"
#include "m2c_macros.h"

extern M2C_UNK D_80121378;

M2C_UNK *func_0028EA40(u32 arg0) {
    M2C_UNK *var_v1;
    u16 temp_a1;

    var_v1 = &D_80121378;
    if (&D_80121378 != NULL) {
loop_1:
        temp_a1 = M2C_FIELD(var_v1, u16 *, 4);
        if ((arg0 < temp_a1) || (arg0 >= (u32) (temp_a1 + M2C_FIELD(var_v1, u16 *, 6)))) {
            var_v1 = M2C_FIELD(var_v1, M2C_UNK **, 0);
            if (var_v1 != NULL) {
                goto loop_1;
            }
        }
    }
    return var_v1;
}
