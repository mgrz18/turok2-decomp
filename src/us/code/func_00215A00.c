#include "common.h"
#include "m2c_macros.h"

s32 func_0020185C(s32, s32, M2C_UNK, s32 *);        /* extern */
s32 *func_0020367C(M2C_UNK, s32, s32, M2C_UNK, s32, M2C_UNK *, M2C_UNK *, s32); /* extern */
M2C_UNK func_002051F4(M2C_UNK, s32 *);              /* extern */
M2C_UNK func_0020BEE8(s32 *);                       /* extern */
extern M2C_UNK D_800A5858;
extern M2C_UNK D_800A5870;
extern M2C_UNK func_0020B4B0;

void func_00215A00(void *arg0) {
    s32 sp20;
    s32 *temp_v0;
    s32 *var_s0;
    s32 temp_s0;

    temp_v0 = func_0020367C(0, M2C_FIELD(arg0, s32 *, 0x34), 0x18, 4, 0, NULL, (void *)((s8 *)&D_800A5858 + 4), 0);
    if (temp_v0 != NULL) {
        temp_s0 = func_0020185C(*temp_v0, M2C_FIELD(arg0, s32 *, 0x34), 1, &sp20);
        func_002051F4(0, temp_v0);
        var_s0 = func_0020367C(0, temp_s0, sp20, 4, 0, &func_0020B4B0, &D_800A5870, 0);
    } else {
        var_s0 = NULL;
    }
    if (var_s0 != NULL) {
        func_0020BEE8(var_s0);
        func_002051F4(0, var_s0);
    }
}
