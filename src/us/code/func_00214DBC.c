#include "common.h"
#include "m2c_macros.h"

s32 func_0020185C(s32, s32, M2C_UNK, void *);       /* extern */
s32 *func_0020367C(M2C_UNK, s32, s32, M2C_UNK, s32, M2C_UNK *, M2C_UNK *, s32); /* extern */
M2C_UNK func_002051F4(M2C_UNK, s32 *);              /* extern */
extern M2C_UNK D_800A5808;
extern M2C_UNK D_800A582C;
extern M2C_UNK func_0020B490;

s32 *func_00214DBC(void *arg0) {
    s32 *temp_v0;

    if (M2C_FIELD(arg0, s32 *, 0x1C) == 0) {
        temp_v0 = func_0020367C(0, M2C_FIELD(arg0, s32 *, 0x18), 0x18, 4, 0, NULL, &D_800A5808, 0);
        if (temp_v0 != NULL) {
            M2C_FIELD(arg0, s32 *, 0x1C) = func_0020185C(*temp_v0, M2C_FIELD(arg0, s32 *, 0x18), 1, arg0 + 0x20);
            func_002051F4(0, temp_v0);
        }
        if (M2C_FIELD(arg0, s32 *, 0x1C) == 0) {
            return NULL;
        }
        goto block_5;
    }
block_5:
    return func_0020367C(0, M2C_FIELD(arg0, s32 *, 0x1C), M2C_FIELD(arg0, s32 *, 0x20), 8, 0, &func_0020B490, &D_800A582C, 0);
}
