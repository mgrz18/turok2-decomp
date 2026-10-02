#include "common.h"
#include "m2c_macros.h"

extern u8 D_800B5E98[];

void func_0041BCD8(s32 arg0, s32 arg1) {
    s32 var_a1;
    u8 temp_v0;
    void *var_v1;

    var_a1 = arg1;
    var_v1 = arg0 + var_a1;
    do {
        temp_v0 = *(((u8 *)(D_800B5E98 + var_a1)));
        var_a1 += 1;
        M2C_FIELD(var_v1, u8 *, 0x33) = temp_v0;
        var_v1 = arg0 + var_a1;
    } while (var_a1 < 8);
}
