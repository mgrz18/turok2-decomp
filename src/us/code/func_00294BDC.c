#include "common.h"
#include "m2c_macros.h"

s32 func_002933E0(s32, s32);

M2C_UNK func_002933B0(s32);                         /* extern */

void func_00294BDC(void *arg0) {
    s32 var_s0;
    void *var_s1;

    var_s1 = arg0;
    var_s0 = M2C_FIELD(var_s1, s32 *, 0x14);
    if (var_s0 != 0) {
        do {
            func_002933B0(var_s0);
            func_002933E0(var_s0, (s32) (var_s1 + 4));
            var_s0 = M2C_FIELD(var_s1, s32 *, 0x14);
        } while (var_s0 != 0);
    }
}
