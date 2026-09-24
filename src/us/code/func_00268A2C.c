#include "common.h"
#include "m2c_macros.h"

s32 func_0022425C(M2C_UNK *, s32);                  /* extern */
extern M2C_UNK D_800F7078;

s32 func_00268A2C(void *arg0) {
    s32 temp_a1;
    s32 var_v0;

    temp_a1 = M2C_FIELD(arg0, s32 *, 0x10);
    var_v0 = 0;
    if (temp_a1 != 0) {
        var_v0 = func_0022425C(&D_800F7078, temp_a1);
    }
    return var_v0;
}
