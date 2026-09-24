#include "common.h"
#include "m2c_macros.h"

s32 func_0020367C(M2C_UNK, s32, s32, M2C_UNK, s32, M2C_UNK *, M2C_UNK *, s32); /* extern */
M2C_UNK func_002051F4(M2C_UNK, s32);                /* extern */
M2C_UNK func_0020BEE8(s32);                         /* extern */
extern M2C_UNK D_800A5CE0;
extern M2C_UNK func_0020B4D0;

void func_0021F3A4(void *arg0) {
    s32 temp_v0;

    temp_v0 = func_0020367C(0, M2C_FIELD(arg0, s32 *, 0xAC), M2C_FIELD(arg0, s32 *, 0xB4), 0, 0, &func_0020B4D0, &D_800A5CE0, 0);
    if (temp_v0 != 0) {
        func_0020BEE8(temp_v0);
        func_002051F4(0, temp_v0);
    }
}
