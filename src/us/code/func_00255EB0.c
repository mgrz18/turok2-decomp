#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_0042EB04();                            /* extern */

void func_00255EB0(void *arg0) {
    M2C_FIELD(arg0, s32 *, 0xBFC) = 0;
    func_0042EB04();
    M2C_FIELD(arg0, s32 *, 0x18) = 0;
    M2C_FIELD(arg0, s32 *, 0x1C) = 0;
    M2C_FIELD(arg0, s32 *, 0x20) = 0;
    M2C_FIELD(arg0, s32 *, 0x1298) = 0;
    M2C_FIELD(arg0, s32 *, 0xC04) = 1;
    M2C_FIELD(arg0, s32 *, 0xD4) = (s32) (M2C_FIELD(arg0, s32 *, 0xD4) & 0xFF7FFFFF);
}
