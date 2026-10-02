#include "common.h"
#include "m2c_macros.h"

extern s32 D_800C2028;
extern s32 D_80130990;

s32 func_004219A8(void *arg0, s32 *arg1) {
    s32 var_a0;
    s32 var_v0;

    if (D_80130990 != 0) {
        var_v0 = D_800C2028;
        var_a0 = 0;
    } else {
        var_a0 = M2C_FIELD(arg0, s32 *, 0x1C);
        var_v0 = (s32) M2C_FIELD(M2C_FIELD(arg0, void **, 0x20), s8 *, 4);
    }
    *arg1 = var_v0;
    return var_a0;
}
