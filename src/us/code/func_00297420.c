#include "common.h"
#include "m2c_macros.h"

M2C_UNK func_00294E70(s32, M2C_UNK, M2C_UNK);       /* extern */
M2C_UNK func_002964A0(s32, M2C_UNK, s32);           /* extern */
M2C_UNK func_00297038(s32, M2C_UNK, s32);           /* extern */

s32 func_00297420(void *arg0, s16 arg1, M2C_UNK arg2, M2C_UNK arg3) {
    s32 temp_a2;
    s32 temp_s0;

    temp_s0 = arg1 * 0x4C;
    func_00294E70(M2C_FIELD(arg0, s32 *, 0x34) + temp_s0 + 0x20, arg2, arg3);
    temp_a2 = M2C_FIELD(arg0, s32 *, 0x34) + temp_s0;
    func_00297038(temp_a2 + 0x20, 1, temp_a2);
    func_002964A0(M2C_FIELD(arg0, s32 *, 0x30), 2, M2C_FIELD(arg0, s32 *, 0x34) + temp_s0 + 0x20);
    return M2C_FIELD(arg0, s32 *, 0x34) + temp_s0 + 0x20;
}
