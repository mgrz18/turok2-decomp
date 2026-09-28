#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00290EC0(s32, s16);                    /* extern */

void func_00275324(void *arg0, f32 arg1) {
    func_00290EC0(M2C_FIELD(arg0, s32 *, 0x14), (s16) (s32) (arg1 * 32767.0f));
    M2C_FIELD(arg0, f32 *, 0x2C) = arg1;
}
