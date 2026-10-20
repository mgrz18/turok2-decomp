#include "common.h"
#include "m2c_macros.h"

s32 func_00267F64(void *arg0) {
    s32 var_v1;
    void *temp_a1;

    temp_a1 = M2C_FIELD(arg0, void **, 0x14);
    if (M2C_FIELD(temp_a1, s32 *, 0) == 1) {
        var_v1 = 0;
        if ((M2C_FIELD(arg0, s32 *, 0x144) <= 0) || (M2C_FIELD(temp_a1, s32 *, 0x48) & 4)) {
            var_v1 = 1;
        }
        return var_v1;
    }
    return 0;
}
