#include "common.h"
#include "m2c_macros.h"

s32 func_00253FE0(void *arg0) {
    s32 var_v0;
    void *temp_v1;

    temp_v1 = M2C_FIELD(arg0, void **, 0x51C);
    var_v0 = 0;
    if (temp_v1 != NULL) {
        var_v0 = M2C_FIELD(temp_v1, s32 *, 0x54C) != 0;
    }
    return var_v0;
}
