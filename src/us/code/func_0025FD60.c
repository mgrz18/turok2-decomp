#include "common.h"
#include "m2c_macros.h"

s32 func_00243414(s32, s32, s32);

void func_0025FD60(s32 arg0, void *arg1) {
    s32 var_a2;

    if (M2C_FIELD(arg1, s8 *, 0xC7) != 0) {
        var_a2 = 0x3C;
        if (M2C_FIELD(arg1, s32 *, 4) != 0) {
            var_a2 = 0x1E;
        }
        func_00243414(arg0, (s32) arg1, var_a2);
    }
}
