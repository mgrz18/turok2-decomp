#include "common.h"
#include "m2c_macros.h"

s32 func_0029DFF0();                                /* extern */
M2C_UNK func_0029E010(s32);                         /* extern */
extern void *D_800B7AC4;

void func_0029AA20(s32 arg0, s32 arg1, s16 arg2) {
    s32 temp_a0;

    temp_a0 = func_0029DFF0();
    M2C_FIELD(D_800B7AC4, s32 *, 0x10) = arg0;
    M2C_FIELD(D_800B7AC4, s32 *, 0x14) = arg1;
    M2C_FIELD(D_800B7AC4, s16 *, 2) = arg2;
    func_0029E010(temp_a0);
}
