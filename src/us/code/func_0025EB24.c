#include "common.h"
#include "m2c_macros.h"

extern f32 D_800A77F4;
extern f32 D_800A77F8;

f32 func_0025EB24(s32 arg0, void *arg1) {
    f32 var_f0;

    var_f0 = D_800A77F4;
    if ((u32) (M2C_FIELD(arg1, u8 *, 0x36) - 1) < 2U) {
        var_f0 = D_800A77F8;
    }
    return var_f0;
}
