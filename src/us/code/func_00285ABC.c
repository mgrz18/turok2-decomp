#include "common.h"
#include "m2c_macros.h"

extern s8 D_800F5EA3;

void func_00285ABC(void) {
    s32 var_v1;
    s8 *var_v0;

    var_v1 = 3;
    var_v0 = &D_800F5EA3;
    do {
        *var_v0 = 0;
        var_v1 -= 1;
        var_v0 -= 1;
    } while (var_v1 >= 0);
}
