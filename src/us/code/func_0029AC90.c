#include "common.h"
#include "m2c_macros.h"

s32 func_0029DFF0();                                /* extern */
M2C_UNK func_0029E010(s32);                         /* extern */
extern void *D_800B7AC4;

void func_0029AC90(s32 arg0) {
    s32 temp_v0;

    temp_v0 = func_0029DFF0();
    M2C_FIELD(D_800B7AC4, s32 *, 4) = arg0;
    M2C_FIELD(D_800B7AC4, u16 *, 0) = (u16) (M2C_FIELD(D_800B7AC4, u16 *, 0) | 0x10);
    func_0029E010(temp_v0);
}
