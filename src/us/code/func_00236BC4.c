#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00233FCC(void *, M2C_UNK);             /* extern */
M2C_UNK func_0023417C(void *);                      /* extern */
M2C_UNK func_00234348(s32, void *, M2C_UNK);        /* extern */
M2C_UNK func_00234928(s32, void *, M2C_UNK);        /* extern */

void func_00236BC4(s32 arg0, void *arg1, M2C_UNK arg2) {
    if (M2C_FIELD(arg1, s32 *, 0x3C) & 8) {
        func_0023417C(arg1);
    }
    if (M2C_FIELD(arg1, s32 *, 0x3C) & 4) {
        func_00233FCC(arg1, arg2);
    }
    if (M2C_FIELD(arg1, s32 *, 0x48) >= 2) {
        if (M2C_FIELD(arg1, s32 *, 0x34) >= 0) {
            func_00234348(arg0, arg1, arg2);
            return;
        }
        func_00234928(arg0, arg1, arg2);
    }
}
