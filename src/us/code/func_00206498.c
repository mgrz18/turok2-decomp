#include "common.h"
#include "m2c_macros.h"

s32 func_00200574(s32, s32);
s32 func_002005D0(s32, s32, s32);

extern void *D_800D81F4;

void func_00206498(s32 arg0, void *arg1) {
    void *var_s0;

    var_s0 = D_800D81F4;
    if (var_s0 != NULL) {
loop_1:
        if ((u32) M2C_FIELD(var_s0, u32 *, 0) > (u32) M2C_FIELD(arg1, u32 *, 0)) {
            func_002005D0((s32) &D_800D81F4, (s32) var_s0, (s32) arg1);
        } else {
            var_s0 = M2C_FIELD(var_s0, void **, 0x24);
            if (var_s0 != NULL) {
                goto loop_1;
            }
        }
        if (var_s0 == NULL) {
            goto block_5;
        }
    } else {
block_5:
        func_00200574((s32) &D_800D81F4, (s32) arg1);
    }
    M2C_FIELD(arg1, s32 *, 0xC) = (s32) (M2C_FIELD(arg1, s32 *, 0xC) | 0x1000);
}
