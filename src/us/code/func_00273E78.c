#include "common.h"
#include "m2c_macros.h"

void func_00273E78(s32 arg0) {
    s32 var_v0;
    void *var_a0;

    var_a0 = arg0 + 4;
    var_v0 = 0x10;
    do {
        M2C_FIELD(var_a0, s32 *, 0x50) = 0;
        var_v0 -= 1;
        var_a0 += 0xCC;
    } while (var_v0 >= 0);
}
