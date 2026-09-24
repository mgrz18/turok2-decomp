#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

void func_00262BE0(s32 arg0, void *arg1, void *arg2) {
    s32 var_v1;

    var_v1 = M2C_FIELD(arg1, s32 *, 4) - M2C_FIELD(arg2, s32 *, 4);
    if (var_v1 < 0) {
        var_v1 = 0;
    }
    M2C_FIELD(arg1, s32 *, 4) = var_v1;
    if (var_v1 == 0) {
        func_00243414(arg0, (s32) arg1, 0x40);
    }
}
