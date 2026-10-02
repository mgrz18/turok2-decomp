#include "common.h"
#include "m2c_macros.h"

s32 func_00416644(s32, s32, s32, s32, s32);

M2C_UNK func_00275904(M2C_UNK);                     /* extern */
M2C_UNK func_00421170();                            /* extern */
extern M2C_UNK D_0043B5C8;
extern M2C_UNK D_80119E2C;

s32 func_004511B4(s32 arg0, void *arg1) {
    s32 temp_s1;

    temp_s1 = M2C_FIELD(arg1, s32 *, 0x1C);
    func_00421170();
    func_00275904(-1);
    func_00416644((s32) &D_80119E2C, (s32) &D_0043B5C8, temp_s1, M2C_FIELD(arg1, s32 *, 0x20), 0);
    return 1;
}
