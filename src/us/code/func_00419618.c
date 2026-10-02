#include "common.h"
#include "m2c_macros.h"

s32 func_00416644(s32, s32, s32, s32, s32);

M2C_UNK func_00421250();                            /* extern */
extern M2C_UNK D_0043B6F0;
extern M2C_UNK D_80119E2C;

s32 func_00419618(s32 arg0, void *arg1) {
    func_00421250();
    func_00416644((s32) &D_80119E2C, (s32) &D_0043B6F0, M2C_FIELD(arg1, s32 *, 0x1C), M2C_FIELD(arg1, s32 *, 0x20), 0);
    return 1;
}
